/**
 * @file CAMRecvHandler.hpp
 * @author Klaus Weinbauer
 * @brief Receive handler for CAMs.
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <CAM.h>
#include <RecvHandler.hpp>

namespace level::cam {

/**
 * @brief Receive handler for CAMs.
 *
 */
class CAMRecvHandler : public RecvHandler<CAM> {
protected:
  virtual void freeMessage(CAM *msg) override {
    ASN_STRUCT_FREE(asn_DEF_CAM, msg);
  }
};

} // namespace level::cam