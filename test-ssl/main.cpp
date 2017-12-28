#include <iostream>
#include <easywsclient.hpp>
#include <cassert>
#include <openssl/ssl.h>
#include <openssl/err.h>

#if (SSLEAY_VERSION_NUMBER >= 0x0907000L)
# include <openssl/conf.h>
#endif
void init_openssl_library(void)
{
    (void)SSL_library_init();
    
    SSL_load_error_strings();
    
    /* ERR_load_crypto_strings(); */
    
    OPENSSL_config(NULL);
    
    /* Include <openssl/opensslconf.h> to get this define */
#if defined (OPENSSL_THREADS)
    fprintf(stdout, "Warning: thread locking is not implemented\n");
#endif
}

using easywsclient::WebSocket;
static WebSocket::pointer ws = NULL;

void handle_message(const std::string & message)
{
    printf(">>> %s\n", message.c_str());
    if (message == "world") { ws->close(); }
}

int main() {
    init_openssl_library();
    ws = WebSocket::from_url("ws://demos.kaazing.com/echo");
    assert(ws);
    ws->send("goodbye");
    ws->send("hello");
    while (ws->getReadyState() != WebSocket::CLOSED) {
        ws->poll(10000);
        ws->dispatch(handle_message);
    }
    delete ws;
    return 0;
}
