/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-02-17     Bernard      First version
 * 2108-05-24     ChenYong     Add socket abstraction layer
 */

#include <rtthread.h>
#ifdef ENABLE_SAL
#include <netdb.h>

struct hostent *gethostbyname(const char *name)
{
    return sal_gethostbyname(name);
}
OS_MODE_EXPORT(gethostbyname);

int gethostbyname_r(const char *name, struct hostent *ret, char *buf,
                size_t buflen, struct hostent **result, int *h_errnop)
{
    return sal_gethostbyname_r(name, ret, buf, buflen, result, h_errnop);
}
OS_MODE_EXPORT(gethostbyname_r);

void freeaddrinfo(struct addrinfo *ai)
{
    sal_freeaddrinfo(ai);
}
OS_MODE_EXPORT(freeaddrinfo);

int getaddrinfo(const char *nodename,
       const char *servname,
       const struct addrinfo *hints,
       struct addrinfo **res)
{
    return sal_getaddrinfo(nodename, servname, hints, res);
}
OS_MODE_EXPORT(getaddrinfo);
#endif
