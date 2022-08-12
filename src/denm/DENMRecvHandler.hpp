/**
 * @file DENMRecvHandler.hpp
 * @author Klaus Weinbauer
 * @brief Receive handler for DENMs.
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <DENM.h>
#include <RecvHandler.hpp>

namespace level::denm {

/**
 * @brief Receive handler for DENMs.
 *
 */
class DENMRecvHandler : public RecvHandler<DENM> {
protected:
  virtual void freeMessage(DENM *msg) override {
    ASN_STRUCT_FREE(asn_DEF_DENM, msg);
  }
};

} // namespace level::denm