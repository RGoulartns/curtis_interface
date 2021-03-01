#include <curtis_interface/curtis_interface.h>

namespace curtis_interface
{
  CurtisInterface::CurtisInterface(uint8_t nodeId)
  : cobTransmitPDO_(transmitPDO_ + nodeId)
  , cobReceivePDO_(receivePDO_ + nodeId)
  , cobReceiveSDO_(receiveSDO_ + nodeId)
  {
    nodeId_ = nodeId;

    canInterface_ = new can_interface::CanInterface();
    
    //setup curtis controller
    canInterface_->sendMessage(cobReceiveSDO_, canDataKsi.size(), canDataKsi);
    usleep(200000);

    canInterface_->sendMessage(cobReceiveSDO_, canDataPdoTimeout.size(), canDataPdoTimeout);
    usleep(200000);

    canDataOpsMode.at(1) = nodeId_;
    canInterface_->sendMessage(0x00, canDataOpsMode.size(), canDataOpsMode);
    
  }

  CurtisInterface::~CurtisInterface()
  {
    delete canInterface_;
  }

  void CurtisInterface::setThrottle(double t)
  {
    throttle_ = throttleToRange(t);

    canDataThrottle.at(2) = throttle_ & 255;
    canDataThrottle.at(3) = (throttle_ >> 8) & 255;
    canInterface_->sendMessage(cobReceivePDO_, canDataThrottle.size(), canDataThrottle);
  }

  int CurtisInterface::getRPM()
  {
    message_.clear();
    if(canInterface_->readMessage(cobTransmitPDO_, message_))
    {
      rpm_ = (message_.at(5) << 8) + message_.at(4);
      if(rpm_ > 30000)
        rpm_ -= 65536;
    }
    else
      rpm_ = 0;

    return rpm_;
  }

  int CurtisInterface::throttleToRange(double &t)
  {
    return 32767*t + 65535*(t < 0);
  }
}

