#include "SmartMeterLineParser.h"

void SmartMeterLineParser::parse(char* line, MeterReading& meterReading) {

  float result;
  uint8_t discardGas = 0;

  // 1-0:1.8.1 = Usage low rate
  if (sscanf(line, "1-0:1.8.1(%f", &result) == 1) {
    meterReading.powerDeliverLow = resultToInt(result);
  }

  // 1-0:1.8.2 = Usage high rate
  else if (sscanf(line, "1-0:1.8.2(%f", &result) == 1) {
    meterReading.powerDeliverHigh = resultToInt(result);
  }

  // 1-0:2.8.1 = Returned low rate
  else if (sscanf(line, "1-0:2.8.1(%f", &result) == 1) {
    meterReading.powerReturnLow = resultToInt(result);
  }

  // 1-0:2.8.2 = Returned high rate
  else if (sscanf(line, "1-0:2.8.2(%f", &result) == 1) {
    meterReading.powerReturnHigh = resultToInt(result);
  }

  // 1-0:1.7.0 = Current usage
  else if (sscanf(line, "1-0:1.7.0(%f", &result) == 1) {
    auto shortage = resultToInt(result);
    if (shortage > 0) {
      meterReading.powerUsage = shortage;
    }
  }

  // 1-0:2.7.0 = Current return
  else if (sscanf(line, "1-0:2.7.0(%f", &result) == 1) {
    auto oversupply = resultToInt(result);
    if (oversupply > 0) {
      meterReading.powerUsage = -oversupply;
    }
  }

  // 0-1:24.2.1 = Gas Summer
  else if (sscanf(line, "0-1:24.2.1(%ldS)(%f*m3", &discardGas, &result) == 2) {
    meterReading.gas = resultToInt(result);
  }
  // 0-1:24.2.1 = Gas Winter
  else if (sscanf(line, "0-1:24.2.1(%ldW)(%f*m3", &discardGas, &result) == 2) {
    meterReading.gas = resultToInt(result);
  }
  // Fallback Gas DSMR2.2
  else if (sscanf(line, "(%f)", &result) == 1) {
    meterReading.gas = resultToInt(result);
  }
}

uint32_t SmartMeterLineParser::resultToInt(float& result) {
    return uint32_t(result*1000);
}
