#ifndef CURTIS_INTERFACE__CURTIS_INTERFACE_H
#define CURTIS_INTERFACE__CURTIS_INTERFACE_H

#include <can_interface/can_interface.h>
#include <vector>

namespace curtis_interface
{
  class CurtisInterface
  {
    public:
      CurtisInterface(uint8_t nodeId);

      virtual ~CurtisInterface();

      void setThrottle(double t);
      int getRPM();
      int throttleToRange(double &t);

      // templates for messages
      const std::vector<uint8_t> canDataKsi = {0x22, 0x3e, 0x30, 0x00, 0x02};
      const std::vector<uint8_t> canDataPdoTimeout = {0x22, 0x49, 0x31, 0x00, 0x32};
      std::vector<uint8_t> canDataOpsMode = {0x01, 0x00};
      std::vector<uint8_t> canDataThrottle = {0x00, 0x00, 0x00, 0x00};

    private:
      // Range 0 to 65535
      uint16_t throttle_;
      
      // Can bus node ids for each controller
      uint8_t nodeId_;

      // stores any message received
      std::vector<uint8_t> message_;
      uint16_t rpm_;

      // Can bus function codes used
      const uint16_t transmitPDO_ = 0x3 << 7;
      const uint16_t receivePDO_ = 0x4 << 7; //0x2??
      const uint16_t receiveSDO_ = 0xC << 7; //0x6??

      const uint16_t cobTransmitPDO_;
      const uint16_t cobReceivePDO_;
      const uint16_t cobReceiveSDO_;

      can_interface::CanInterface* canInterface_;
  };
}

#endif // CURTIS_INTERFACE__CURTIS_INTERFACE_H