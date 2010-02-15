/*
 * Copyright (c) 2003 Maxim Sobolev <sobomax@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: execinfo.c,v 1.3 2004/07/19 05:21:09 sobomax Exp $
 */

#ifndef _CXXBACKTRACE_H_
#define _CXXBACKTRACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <cxxabi.h>

#define MAX_FRAMES 63

static inline void
cxxbacktrace_dump_symbols(void **buffer, int size)
{
    int i, offset;
    Dl_info info;

    for (i = 0; i < size; i++) {
        if (dladdr(buffer[i], &info) != 0) {
			int status;
			char *demangled;
			
            if (info.dli_sname == NULL)
                info.dli_sname = "???";
            if (info.dli_saddr == NULL)
                info.dli_saddr = buffer[i];
            offset = (unsigned long)buffer[i] - (unsigned long)info.dli_saddr;
			demangled = abi::__cxa_demangle(info.dli_sname, 0, 0, &status);
            printf("%p <%s+%d> at %s\n",
				   buffer[i], demangled ? demangled : info.dli_sname, offset, info.dli_fname);
        } else {
            printf("%p\n", buffer[i]);
        }
    }
}

static inline void
cxxbacktrace_here(void)
{
    // storage array for stack trace address data
    void* addrlist[MAX_FRAMES+1];
    // retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

	printf("backtrace:\n");
    if (addrlen == 0) {
		printf("  <empty, possibly corrupt>\n");
		return;
    }
    cxxbacktrace_dump_symbols(addrlist, addrlen);
}
#endif /* _CXXBACKTRACE_H_ */
