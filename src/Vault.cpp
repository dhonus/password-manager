//
// Created by daniel on 27.5.22.
//

#include "Vault.h"
#include <cstdio>
#include <sodium.h>
#include <cstring>
#include <string>
#include <iostream>
#include <stdlib.h>

// file decryption code implemented using https://libsodium.gitbook.io/doc/secret-key_cryptography/secretstream

#define CHUNK_SIZE 128
#define RED     "\033[31m"
#define RESET   "\033[0m"

std::optional<std::stringstream> Vault::decrypt(std::string t_source_file)
{
    this->filename = t_source_file;
    std::cout << "Decrypting " << this->filename << "\n";

    #define KEY_LEN crypto_box_SEEDBYTES
    unsigned char key[KEY_LEN];
    FILE          *input_file;
    input_file = fopen(this->filename.c_str(), "rb");
    if (input_file == NULL) {
        perror("Failed: ");
        return std::nullopt;
    }
    // read salt to perform vault decryption
    unsigned char salt[crypto_pwhash_SALTBYTES];

    if (fread(salt, 1, sizeof(salt), input_file) != sizeof(salt)) {
        std::cout << "Failed to read the vault.\n";
        return std::nullopt;
    }

    // begin decryption process
    if (crypto_pwhash(key,
            sizeof key, this->vault_password.c_str(),
            strlen(this->vault_password.c_str()), salt,
            crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE,
            crypto_pwhash_ALG_DEFAULT) != 0) {
                std::cout << "Ran out of memory. Your vault may be corrupted\n";
                return std::nullopt;
    }

    unsigned char  buf_in[CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES];
    unsigned char  buf_out[CHUNK_SIZE];
    unsigned char  header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
    crypto_secretstream_xchacha20poly1305_state st;

    unsigned long long out_len;
    size_t rlen;
    int eof;
    unsigned char tag;
    std::stringstream ret_ss;

    fread(header, 1, sizeof header, input_file);
    if (crypto_secretstream_xchacha20poly1305_init_pull(&st, header, key) != 0) {
        std::cout << "Failed to read the vault.\n";
        return std::nullopt;
    }
    while (!eof){
        rlen = fread(buf_in, 1, sizeof buf_in, input_file);
        eof = feof(input_file);
        if (crypto_secretstream_xchacha20poly1305_pull(&st, buf_out, &out_len, &tag, buf_in, rlen, NULL, 0) != 0) {
            if (this->file_exists(filename)){
                filename += "_new";
                std::cout << RED << "Wrong password. Creating a new vault at " << filename << RESET << "\n";
                return std::nullopt;
            }
            goto ret; // corrupted chunk; still acceptable
        }
        if (tag == crypto_secretstream_xchacha20poly1305_TAG_FINAL && ! eof) {
            goto ret; // premature end (end of file reached before the end of the stream)
        }
        // add to returned stream
        ret_ss << buf_out << " ";
        if(!strcmp(reinterpret_cast<const char *>(buf_out), "</PassRoot>")){
            goto ret;
        }
    };

ret:
    fclose(input_file);
    return ret_ss;
}


// method creates internal data struct from vault file
std::shared_ptr<Data> Vault::assembleFileFromData(std::string t_path)
{
    std::shared_ptr<Data> data = std::make_shared<Data>();

    pugi::xml_document doc;
    // decrypt vault; if fails (bad pass, ...) use create a new empty vault
    auto stream = decrypt(t_path).value_or(std::stringstream(""));

    pugi::xml_parse_result result = doc.load(stream);

    // if the xml stream errors, return new vault
    if (!result) {
        return data;
    }

    // assemble from xml
    for (pugi::xml_node tool: doc.child("PassRoot"))
    {
        std::shared_ptr<Password> p = std::make_shared<Password>(
                tool.attribute("name").as_string(), tool.attribute("username").as_string(), tool.attribute("password").as_string(""));
        if (data->insert(std::move(p)) != 0){
            std::cout << "!! password insert failed\n";
            break;
        }
    }
    return data;
}

// method saves internal data to xml file. Creates empty xml file when no data is supplied.
int Vault::save(std::shared_ptr<Data> t_data)
{
    std::stringstream ss;

    // if the vault has data assemble xml and save
    if(!t_data->getData().empty()){
        pugi::xml_document doc_out;
        pugi::xml_node node = doc_out.prepend_child("PassRoot");
        for (auto & data : t_data->getData()){
            // add param node before the description
            pugi::xml_node param = node.append_child("Pass");

            // add attributes to param node
            param.append_attribute("name") = data->getName().c_str();
            param.append_attribute("username") = data->getUsername().c_str();
            param.append_attribute("password") = data->getPassword().c_str();
            param.append_child(pugi::node_pcdata).set_value("");
        }
        doc_out.save(ss);
    }
    else {
        pugi::xml_document doc_out;
        pugi::xml_node node = doc_out.append_child("PassRoot");
        node.set_value("");
        doc_out.save(ss);
        doc_out.print(std::cout);
    }

    #define KEY_LEN crypto_box_SEEDBYTES

    unsigned char salt[crypto_pwhash_SALTBYTES];
    unsigned char key[KEY_LEN];

    // salt generation. New salt on every save.
    randombytes_buf(salt, sizeof salt);

    if (crypto_pwhash (key, sizeof key, this->vault_password.c_str(), strlen( this->vault_password.c_str() ), salt,
                 crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE,
                 crypto_pwhash_ALG_DEFAULT) != 0) {
    }

    unsigned char  buf_in[CHUNK_SIZE];
    unsigned char  buf_out[CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES];
    unsigned char  header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
    crypto_secretstream_xchacha20poly1305_state st;
    FILE          *fp_t;
    unsigned long long out_len;
    size_t         rlen;

    fp_t = fopen(this->filename.c_str(), "wb");
    if (fwrite(salt, 1, sizeof salt, fp_t) != sizeof salt) {
        fprintf(stderr, "ERROR: writing salt failed\n");
        return EXIT_FAILURE;
    }
    crypto_secretstream_xchacha20poly1305_init_push(&st, header, key);
    fwrite(header, 1, sizeof header, fp_t);
    do {
        ss >> buf_in;
        rlen = sizeof buf_in;
        crypto_secretstream_xchacha20poly1305_push(&st, buf_out, &out_len, buf_in, rlen, NULL, 0, 0);
        fwrite(buf_out, 1, (size_t) out_len, fp_t);
    } while (ss.good());
    fclose(fp_t);

    return 0;
}

Vault::Vault()
{
    if (sodium_init() != 0) {
        std::cout << "FAILED TO RUN SODIUM; FATAL ERROR";
    }
    std::cout << "Please enter your password: ";
    getline(std::cin, this->vault_password);
}

bool Vault::file_exists(const std::string &file) {
    std::ifstream infile(file);
    return infile.good();
}

std::string Vault::getPath() {
    return this->filename;
}

void Vault::setPath(const std::string &t_path) {
    this->filename = t_path;
}
