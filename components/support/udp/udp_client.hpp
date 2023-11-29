
#ifndef _UDP_CLIENT_H_
#define _UDP_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cmsis_os.h>

#ifdef __cplusplus
}
#endif

class Udp
{
public:
    Udp();
    ~Udp() = default;

    void udp_open(uint32_t port);

    void udp_close();

    int udp_send(const char *data, size_t len, const char *ip = "192.168.1.11", uint16_t port = 5000);

    int udp_recv(char *data, size_t len);

private:
	int sockfd_;
};


#endif /* _UDP_CLIENT_H_ */

