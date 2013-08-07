// Copyright (c) 2013, Cornell University
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of HyperDex nor the names of its contributors may be
//       used to endorse or promote products derived from this software without
//       specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// HyperDex
#include <hyperdex/client.h>
#include "visibility.h"
#include "common/macros.h"
#include "client/client.h"

#define C_WRAP_EXCEPT(X) \
    hyperdex::client* cl = reinterpret_cast<hyperdex::client*>(_cl); \
    try \
    { \
        X \
    } \
    catch (po6::error& e) \
    { \
        errno = e; \
        *status = HYPERDEX_CLIENT_EXCEPTION; \
        cl->set_error_message("unhandled exception was thrown"); \
        return -1; \
    } \
    catch (std::bad_alloc& ba) \
    { \
        errno = ENOMEM; \
        *status = HYPERDEX_CLIENT_NOMEM; \
        cl->set_error_message("out of memory"); \
        return -1; \
    } \
    catch (...) \
    { \
        *status = HYPERDEX_CLIENT_EXCEPTION; \
        cl->set_error_message("unhandled exception was thrown"); \
        return -1; \
    }

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

HYPERDEX_API struct hyperdex_client*
hyperdex_client_create(const char* coordinator, uint16_t port)
{
    try
    {
        return reinterpret_cast<struct hyperdex_client*>(new hyperdex::client(coordinator, port));
    }
    catch (po6::error& e)
    {
        errno = e;
        return NULL;
    }
    catch (std::bad_alloc& ba)
    {
        errno = ENOMEM;
        return NULL;
    }
    catch (...)
    {
        return NULL;
    }
}

HYPERDEX_API void
hyperdex_client_destroy(struct hyperdex_client* client)
{
    delete reinterpret_cast<hyperdex::client*>(client);
}

HYPERDEX_API const char*
hyperdex_client_error_message(struct hyperdex_client* _cl)
{
    hyperdex::client* cl = reinterpret_cast<hyperdex::client*>(_cl);
    return cl->error_message();
}

HYPERDEX_API const char*
hyperdex_client_error_location(struct hyperdex_client* _cl)
{
    hyperdex::client* cl = reinterpret_cast<hyperdex::client*>(_cl);
    return cl->error_location();
}

HYPERDEX_API const char*
hyperdex_client_returncode_to_string(enum hyperdex_client_returncode status)
{
    switch (status)
    {
        CSTRINGIFY(HYPERDEX_CLIENT_SUCCESS);
        CSTRINGIFY(HYPERDEX_CLIENT_NOTFOUND);
        CSTRINGIFY(HYPERDEX_CLIENT_SEARCHDONE);
        CSTRINGIFY(HYPERDEX_CLIENT_CMPFAIL);
        CSTRINGIFY(HYPERDEX_CLIENT_READONLY);
        CSTRINGIFY(HYPERDEX_CLIENT_UNKNOWNSPACE);
        CSTRINGIFY(HYPERDEX_CLIENT_COORDFAIL);
        CSTRINGIFY(HYPERDEX_CLIENT_SERVERERROR);
        CSTRINGIFY(HYPERDEX_CLIENT_POLLFAILED);
        CSTRINGIFY(HYPERDEX_CLIENT_OVERFLOW);
        CSTRINGIFY(HYPERDEX_CLIENT_RECONFIGURE);
        CSTRINGIFY(HYPERDEX_CLIENT_TIMEOUT);
        CSTRINGIFY(HYPERDEX_CLIENT_UNKNOWNATTR);
        CSTRINGIFY(HYPERDEX_CLIENT_DUPEATTR);
        CSTRINGIFY(HYPERDEX_CLIENT_NONEPENDING);
        CSTRINGIFY(HYPERDEX_CLIENT_DONTUSEKEY);
        CSTRINGIFY(HYPERDEX_CLIENT_WRONGTYPE);
        CSTRINGIFY(HYPERDEX_CLIENT_NOMEM);
        CSTRINGIFY(HYPERDEX_CLIENT_BADCONFIG);
        CSTRINGIFY(HYPERDEX_CLIENT_DUPLICATE);
        CSTRINGIFY(HYPERDEX_CLIENT_INTERRUPTED);
        CSTRINGIFY(HYPERDEX_CLIENT_CLUSTER_JUMP);
        CSTRINGIFY(HYPERDEX_CLIENT_COORD_LOGGED);
        CSTRINGIFY(HYPERDEX_CLIENT_OFFLINE);
        CSTRINGIFY(HYPERDEX_CLIENT_INTERNAL);
        CSTRINGIFY(HYPERDEX_CLIENT_EXCEPTION);
        CSTRINGIFY(HYPERDEX_CLIENT_GARBAGE);
        default:
            return "unknown hyperdex_client_returncode";
    }
}

HYPERDEX_API enum hyperdatatype
hyperdex_client_attribute_type(struct hyperdex_client* _cl,
                               const char* space, const char* name,
                               enum hyperdex_client_returncode* status)
{
    hyperdex::client* cl = reinterpret_cast<hyperdex::client*>(_cl); \

    try
    {
        return cl->attribute_type(space, name, status);
    }
    catch (po6::error& e)
    {
        errno = e;
        *status = HYPERDEX_CLIENT_EXCEPTION;
        cl->set_error_message("unhandled exception was thrown");
        return HYPERDATATYPE_GARBAGE;
    }
    catch (std::bad_alloc& ba)
    {
        errno = ENOMEM;
        *status = HYPERDEX_CLIENT_NOMEM;
        cl->set_error_message("out of memory");
        return HYPERDATATYPE_GARBAGE;
    }
    catch (...)
    {
        *status = HYPERDEX_CLIENT_EXCEPTION;
        cl->set_error_message("unhandled exception was thrown");
        return HYPERDATATYPE_GARBAGE;
    }
}

HYPERDEX_API void
hyperdex_client_destroy_attrs(const struct hyperdex_client_attribute* attrs, size_t /*attrs_sz*/)
{
    free(const_cast<struct hyperdex_client_attribute*>(attrs));
}

HYPERDEX_API int64_t
hyperdex_client_get(struct hyperdex_client* _cl,
                    const char* space,
                    const char* key, size_t key_sz,
                    hyperdex_client_returncode* status,
                    const struct hyperdex_client_attribute** attrs, size_t* attrs_sz)
{
    C_WRAP_EXCEPT(
    return cl->get(space, key, key_sz, status, attrs, attrs_sz);
    );
}

HYPERDEX_API int64_t
hyperdex_client_put(struct hyperdex_client* _cl,
                    const char* space,
                    const char* key, size_t key_sz,
                    const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                    hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(put), strlen(XSTR(put)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_put(struct hyperdex_client* _cl,
                         const char* space,
                         const char* key, size_t key_sz,
                         const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                         const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                         hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_put), strlen(XSTR(cond_put)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_put_if_not_exist(struct hyperdex_client* _cl,
                                 const char* space,
                                 const char* key, size_t key_sz,
                                 const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                                 hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(put_if_not_exist), strlen(XSTR(put_if_not_exist)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_del(struct hyperdex_client* _cl,
                    const char* space,
                    const char* key, size_t key_sz,
                    hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(del), strlen(XSTR(del)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, NULL, 0, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_del(struct hyperdex_client* _cl,
                         const char* space,
                         const char* key, size_t key_sz,
                         const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                         hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_del), strlen(XSTR(cond_del)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, NULL, 0, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_atomic_add(struct hyperdex_client* _cl,
                           const char* space,
                           const char* key, size_t key_sz,
                           const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                           hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(atomic_add), strlen(XSTR(atomic_add)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_atomic_add(struct hyperdex_client* _cl,
                                const char* space,
                                const char* key, size_t key_sz,
                                const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                                hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_atomic_add), strlen(XSTR(cond_atomic_add)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_atomic_sub(struct hyperdex_client* _cl,
                           const char* space,
                           const char* key, size_t key_sz,
                           const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                           hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(atomic_sub), strlen(XSTR(atomic_sub)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_atomic_sub(struct hyperdex_client* _cl,
                                const char* space,
                                const char* key, size_t key_sz,
                                const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                                hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_atomic_sub), strlen(XSTR(cond_atomic_sub)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_atomic_mul(struct hyperdex_client* _cl,
                           const char* space,
                           const char* key, size_t key_sz,
                           const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                           hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(atomic_mul), strlen(XSTR(atomic_mul)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_atomic_mul(struct hyperdex_client* _cl,
                                const char* space,
                                const char* key, size_t key_sz,
                                const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                                hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_atomic_mul), strlen(XSTR(cond_atomic_mul)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_atomic_div(struct hyperdex_client* _cl,
                           const char* space,
                           const char* key, size_t key_sz,
                           const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                           hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(atomic_div), strlen(XSTR(atomic_div)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_atomic_div(struct hyperdex_client* _cl,
                                const char* space,
                                const char* key, size_t key_sz,
                                const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                                hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_atomic_div), strlen(XSTR(cond_atomic_div)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_atomic_mod(struct hyperdex_client* _cl,
                           const char* space,
                           const char* key, size_t key_sz,
                           const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                           hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(atomic_mod), strlen(XSTR(atomic_mod)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_atomic_mod(struct hyperdex_client* _cl,
                                const char* space,
                                const char* key, size_t key_sz,
                                const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                                hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_atomic_mod), strlen(XSTR(cond_atomic_mod)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_atomic_and(struct hyperdex_client* _cl,
                           const char* space,
                           const char* key, size_t key_sz,
                           const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                           hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(atomic_and), strlen(XSTR(atomic_and)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_atomic_and(struct hyperdex_client* _cl,
                                const char* space,
                                const char* key, size_t key_sz,
                                const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                                hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_atomic_and), strlen(XSTR(cond_atomic_and)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_atomic_or(struct hyperdex_client* _cl,
                          const char* space,
                          const char* key, size_t key_sz,
                          const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                          hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(atomic_or), strlen(XSTR(atomic_or)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_atomic_or(struct hyperdex_client* _cl,
                               const char* space,
                               const char* key, size_t key_sz,
                               const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                               const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                               hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_atomic_or), strlen(XSTR(cond_atomic_or)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_atomic_xor(struct hyperdex_client* _cl,
                           const char* space,
                           const char* key, size_t key_sz,
                           const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                           hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(atomic_xor), strlen(XSTR(atomic_xor)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_atomic_xor(struct hyperdex_client* _cl,
                                const char* space,
                                const char* key, size_t key_sz,
                                const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                                hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_atomic_xor), strlen(XSTR(cond_atomic_xor)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_string_prepend(struct hyperdex_client* _cl,
                               const char* space,
                               const char* key, size_t key_sz,
                               const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                               hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(string_prepend), strlen(XSTR(string_prepend)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_string_prepend(struct hyperdex_client* _cl,
                                    const char* space,
                                    const char* key, size_t key_sz,
                                    const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                    const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                                    hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_string_prepend), strlen(XSTR(cond_string_prepend)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_string_append(struct hyperdex_client* _cl,
                              const char* space,
                              const char* key, size_t key_sz,
                              const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                              hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(string_append), strlen(XSTR(string_append)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_string_append(struct hyperdex_client* _cl,
                                   const char* space,
                                   const char* key, size_t key_sz,
                                   const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                   const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                                   hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_string_append), strlen(XSTR(cond_string_append)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_list_lpush(struct hyperdex_client* _cl,
                           const char* space,
                           const char* key, size_t key_sz,
                           const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                           hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(list_lpush), strlen(XSTR(list_lpush)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_list_lpush(struct hyperdex_client* _cl,
                                const char* space,
                                const char* key, size_t key_sz,
                                const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                                hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_list_lpush), strlen(XSTR(cond_list_lpush)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_list_rpush(struct hyperdex_client* _cl,
                           const char* space,
                           const char* key, size_t key_sz,
                           const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                           hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(list_rpush), strlen(XSTR(list_rpush)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_list_rpush(struct hyperdex_client* _cl,
                                const char* space,
                                const char* key, size_t key_sz,
                                const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                                hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_list_rpush), strlen(XSTR(cond_list_rpush)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_set_add(struct hyperdex_client* _cl,
                        const char* space,
                        const char* key, size_t key_sz,
                        const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                        hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(set_add), strlen(XSTR(set_add)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_set_add(struct hyperdex_client* _cl,
                             const char* space,
                             const char* key, size_t key_sz,
                             const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                             const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                             hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_set_add), strlen(XSTR(cond_set_add)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_set_remove(struct hyperdex_client* _cl,
                           const char* space,
                           const char* key, size_t key_sz,
                           const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                           hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(set_remove), strlen(XSTR(set_remove)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_set_remove(struct hyperdex_client* _cl,
                                const char* space,
                                const char* key, size_t key_sz,
                                const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                                hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_set_remove), strlen(XSTR(cond_set_remove)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_set_intersect(struct hyperdex_client* _cl,
                              const char* space,
                              const char* key, size_t key_sz,
                              const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                              hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(set_intersect), strlen(XSTR(set_intersect)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_set_intersect(struct hyperdex_client* _cl,
                                   const char* space,
                                   const char* key, size_t key_sz,
                                   const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                   const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                                   hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_set_intersect), strlen(XSTR(cond_set_intersect)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_set_union(struct hyperdex_client* _cl,
                          const char* space,
                          const char* key, size_t key_sz,
                          const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                          hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(set_union), strlen(XSTR(set_union)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_set_union(struct hyperdex_client* _cl,
                               const char* space,
                               const char* key, size_t key_sz,
                               const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                               const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                               hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_set_union), strlen(XSTR(cond_set_union)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_map_add(struct hyperdex_client* _cl,
                        const char* space,
                        const char* key, size_t key_sz,
                        const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                        hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(map_add), strlen(XSTR(map_add)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_map_add(struct hyperdex_client* _cl,
                             const char* space,
                             const char* key, size_t key_sz,
                             const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                             const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                             hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_map_add), strlen(XSTR(cond_map_add)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_map_remove(struct hyperdex_client* _cl,
                           const char* space,
                           const char* key, size_t key_sz,
                           const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                           hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(map_remove), strlen(XSTR(map_remove)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_map_remove(struct hyperdex_client* _cl,
                            const char* space,
                            const char* key, size_t key_sz,
                            const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                            const struct hyperdex_client_attribute* attrs, size_t attrs_sz,
                            hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_map_remove), strlen(XSTR(cond_map_remove)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, attrs, attrs_sz, NULL, 0, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_map_atomic_add(struct hyperdex_client* _cl,
                               const char* space,
                               const char* key, size_t key_sz,
                               const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                               hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(map_atomic_add), strlen(XSTR(map_atomic_add)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_map_atomic_add(struct hyperdex_client* _cl,
                                    const char* space,
                                    const char* key, size_t key_sz,
                                    const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                    const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                                    hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_map_atomic_add), strlen(XSTR(cond_map_atomic_add)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_map_atomic_sub(struct hyperdex_client* _cl,
                               const char* space,
                               const char* key, size_t key_sz,
                               const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                               hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(map_atomic_sub), strlen(XSTR(map_atomic_sub)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_map_atomic_sub(struct hyperdex_client* _cl,
                                    const char* space,
                                    const char* key, size_t key_sz,
                                    const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                    const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                                    hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_map_atomic_sub), strlen(XSTR(cond_map_atomic_sub)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_map_atomic_mul(struct hyperdex_client* _cl,
                               const char* space,
                               const char* key, size_t key_sz,
                               const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                               hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(map_atomic_mul), strlen(XSTR(map_atomic_mul)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_map_atomic_mul(struct hyperdex_client* _cl,
                                    const char* space,
                                    const char* key, size_t key_sz,
                                    const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                    const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                                    hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_map_atomic_mul), strlen(XSTR(cond_map_atomic_mul)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_map_atomic_div(struct hyperdex_client* _cl,
                               const char* space,
                               const char* key, size_t key_sz,
                               const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                               hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(map_atomic_div), strlen(XSTR(map_atomic_div)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_map_atomic_div(struct hyperdex_client* _cl,
                                    const char* space,
                                    const char* key, size_t key_sz,
                                    const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                    const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                                    hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_map_atomic_div), strlen(XSTR(cond_map_atomic_div)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_map_atomic_mod(struct hyperdex_client* _cl,
                               const char* space,
                               const char* key, size_t key_sz,
                               const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                               hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(map_atomic_mod), strlen(XSTR(map_atomic_mod)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_map_atomic_mod(struct hyperdex_client* _cl,
                                    const char* space,
                                    const char* key, size_t key_sz,
                                    const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                    const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                                    hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_map_atomic_mod), strlen(XSTR(cond_map_atomic_mod)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_map_atomic_and(struct hyperdex_client* _cl,
                               const char* space,
                               const char* key, size_t key_sz,
                               const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                               hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(map_atomic_and), strlen(XSTR(map_atomic_and)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_map_atomic_and(struct hyperdex_client* _cl,
                                    const char* space,
                                    const char* key, size_t key_sz,
                                    const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                    const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                                    hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_map_atomic_and), strlen(XSTR(cond_map_atomic_and)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_map_atomic_or(struct hyperdex_client* _cl,
                              const char* space,
                              const char* key, size_t key_sz,
                              const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                              hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(map_atomic_or), strlen(XSTR(map_atomic_or)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_map_atomic_or(struct hyperdex_client* _cl,
                                   const char* space,
                                   const char* key, size_t key_sz,
                                   const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                   const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                                   hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_map_atomic_or), strlen(XSTR(cond_map_atomic_or)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_map_atomic_xor(struct hyperdex_client* _cl,
                               const char* space,
                               const char* key, size_t key_sz,
                               const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                               hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(map_atomic_xor), strlen(XSTR(map_atomic_xor)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_map_atomic_xor(struct hyperdex_client* _cl,
                                    const char* space,
                                    const char* key, size_t key_sz,
                                    const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                    const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                                    hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_map_atomic_xor), strlen(XSTR(cond_map_atomic_xor)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_map_string_prepend(struct hyperdex_client* _cl,
                                   const char* space,
                                   const char* key, size_t key_sz,
                                   const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                                   hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(map_string_prepend), strlen(XSTR(map_string_prepend)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_map_string_prepend(struct hyperdex_client* _cl,
                                        const char* space,
                                        const char* key, size_t key_sz,
                                        const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                        const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                                        hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_map_string_prepend), strlen(XSTR(cond_map_string_prepend)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_map_string_append(struct hyperdex_client* _cl,
                                  const char* space,
                                  const char* key, size_t key_sz,
                                  const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                                  hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(map_string_append), strlen(XSTR(map_string_append)));
    return cl->perform_funcall(opinfo, space, key, key_sz, NULL, 0, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_cond_map_string_append(struct hyperdex_client* _cl,
                                       const char* space,
                                       const char* key, size_t key_sz,
                                       const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                       const struct hyperdex_client_map_attribute* mapattrs, size_t mapattrs_sz,
                                       hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    const hyperdex_client_keyop_info* opinfo;
    opinfo = hyperdex_client_keyop_info_lookup(XSTR(cond_map_string_append), strlen(XSTR(cond_map_string_append)));
    return cl->perform_funcall(opinfo, space, key, key_sz, checks, checks_sz, NULL, 0, mapattrs, mapattrs_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_search(struct hyperdex_client* _cl,
                       const char* space,
                       const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                       hyperdex_client_returncode* status,
                       const struct hyperdex_client_attribute** attrs, size_t* attrs_sz)
{
    C_WRAP_EXCEPT(
    return cl->search(space, checks, checks_sz, status, attrs, attrs_sz);
    );
}

HYPERDEX_API int64_t
hyperdex_client_search_describe(struct hyperdex_client* _cl,
                                const char* space,
                                const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                                hyperdex_client_returncode* status,
                                const char** str)
{
    C_WRAP_EXCEPT(
    return cl->search_describe(space, checks, checks_sz, status, str);
    );
}

HYPERDEX_API int64_t
hyperdex_client_sorted_search(struct hyperdex_client* _cl,
                              const char* space,
                              const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                              const char* sort_by, uint64_t limit, int maximize,
                              hyperdex_client_returncode* status,
                              const struct hyperdex_client_attribute** attrs, size_t* attrs_sz)
{
    C_WRAP_EXCEPT(
    return cl->sorted_search(space, checks, checks_sz, sort_by, limit, maximize, status, attrs, attrs_sz);
    );
}

HYPERDEX_API int64_t
hyperdex_client_group_del(struct hyperdex_client* _cl,
                          const char* space,
                          const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                          hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    return cl->group_del(space, checks, checks_sz, status);
    );
}

HYPERDEX_API int64_t
hyperdex_client_count(struct hyperdex_client* _cl,
                      const char* space,
                      const struct hyperdex_client_attribute_check* checks, size_t checks_sz,
                      hyperdex_client_returncode* status, uint64_t* result)
{
    C_WRAP_EXCEPT(
    return cl->count(space, checks, checks_sz, status, result);
    );
}

HYPERDEX_API int64_t
hyperdex_client_loop(struct hyperdex_client* _cl, int timeout,
                     hyperdex_client_returncode* status)
{
    C_WRAP_EXCEPT(
    return cl->loop(timeout, status);
    );
}

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
