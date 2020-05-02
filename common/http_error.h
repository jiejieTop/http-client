/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-15 00:42:16
 * @LastEditTime: 2020-04-27 23:35:05
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _HTTP_ERROR_H_
#define _HTTP_ERROR_H_

typedef enum http_error {
    HTTP_SSL_CERT_ERROR                                     = -0x001C,      /* cetr parse failed */
    HTTP_SOCKET_FAILED_ERROR                                = -0x001B,      /* socket fd failed */
    HTTP_SOCKET_UNKNOWN_HOST_ERROR                          = -0x001A,      /* socket unknown host ip or domain */ 
    HTTP_SET_PUBLISH_DUP_FAILED_ERROR                       = -0x0019,      /* http publish packet set udp bit failed */
    HTTP_CLEAN_SESSION_ERROR                                = -0x0018,      /* http clean session error */
    HTTP_ACK_NODE_IS_EXIST_ERROR                            = -0x0017,      /* http ack list is exist ack node */
    HTTP_ACK_HANDLER_NUM_TOO_MUCH_ERROR                     = -0x0016,      /* http ack handler number is too much */
    HTTP_RESUBSCRIBE_ERROR                                  = -0x0015,      /* http resubscribe error */
    HTTP_SUBSCRIBE_ERROR                                    = -0x0014,      /* http subscribe error */
    HTTP_SEND_PACKET_ERROR                                  = -0x0013,      /* http send a packet */
    HTTP_SERIALIZE_PUBLISH_ACK_PACKET_ERROR                 = -0x0012,      /* http serialize publish ack packet error */
    HTTP_PUBLISH_PACKET_ERROR                               = -0x0011,      /* http publish packet error */
    HTTP_RECONNECT_TIMEOUT_ERROR                            = -0x0010,      /* http try reconnect, but timeout */
    HTTP_SUBSCRIBE_NOT_ACK_ERROR                            = -0x000F,      /* http subscribe, but not ack */
    HTTP_NOT_CONNECT_ERROR                                  = -0x000E,      /* http not connect */
    HTTP_SUBSCRIBE_ACK_PACKET_ERROR                         = -0x000D,      /* http subscribe, but ack packet error */
    HTTP_UNSUBSCRIBE_ACK_PACKET_ERROR                       = -0x000C,      /* http unsubscribe, but ack packet error */
    HTTP_PUBLISH_ACK_PACKET_ERROR                           = -0x000B,      /* http pubilsh ack packet error */
    HTTP_PUBLISH_ACK_TYPE_ERROR                             = -0x000A,      /* http pubilsh ack type error */
    HTTP_PUBREC_PACKET_ERROR                                = -0x0009,      /* http pubrec packet error */
    HTTP_BUFFER_TOO_SHORT_ERROR                             = -0x0008,      /* http buffer too short */
    HTTP_NOTHING_TO_READ_ERROR                              = -0x0007,      /* http nothing to read */
    HTTP_SUBSCRIBE_QOS_ERROR                                = -0x0006,      /* http subsrcibe qos error */
    HTTP_BUFFER_OVERFLOW_ERROR                              = -0x0005,      /* http buffer overflow */
    HTTP_CONNECT_FAILED_ERROR                               = -0x0004,      /* http connect failed */
    HTTP_MEM_NOT_ENOUGH_ERROR                               = -0x0003,      /* http memory not enough */
    HTTP_NULL_VALUE_ERROR                                   = -0x0002,      /* http value is null */
    HTTP_FAILED_ERROR                                       = -0x0001,      /* failed */
    HTTP_SUCCESS_ERROR                                      = 0x0000        /* success */
} http_error_t;

#define RETURN_ERROR(x) { return x; }

#endif
