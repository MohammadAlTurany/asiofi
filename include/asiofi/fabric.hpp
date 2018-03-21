/********************************************************************************
 *    Copyright (C) 2018 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ASIOFI_FABRIC_HPP
#define ASIOFI_FABRIC_HPP

#include <asiofi/errno.hpp>
#include <cstdint>
#include <ostream>
#include <rdma/fabric.h>

namespace asiofi
{

/**
 * @struct info fabric.hpp <asiofi/fabric.hpp>
 * @brief wraps the fi_info struct
 */
  struct info
  {
    /// default ctor
    info() : m_info(fi_allocinfo()) { }

    /// ctor
    explicit info(int version, const char* node, const char* service,
      uint64_t flags, const info& hints)
    {
      auto rc = fi_getinfo(version, node, service, flags, hints.get(), &m_info);
      if (rc != 0)
        throw runtime_error("Failed querying fi_getinfo, reason: ", fi_strerror(rc));
    }

    /// copy ctor
    explicit info(const info& rh) : m_info(fi_dupinfo(rh.get())) { }

    /// move ctor
    explicit info(info&& rh)
    {
      m_info = rh.m_info;
      rh.m_info = nullptr;
    }

    /// dtor
    ~info() { fi_freeinfo(m_info); }

    friend auto operator<<(std::ostream& os, const info& info) -> std::ostream&
    {
      return os << fi_tostr(m_info, FI_TYPE_INFO);
    }

    private:
    fi_info* m_info;

    auto get() const -> const fi_info* { return m_info; }
  }; /* struct info */

} /* namespace asiofi */

#endif /* ASIOFI_FABRIC_HPP */