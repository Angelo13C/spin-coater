/*
 * MCT8316Z.h
 *
 *  Created on: Jun 13, 2024
 *      Author: Angelo
 */

#ifndef MCT8316Z_H_
#define MCT8316Z_H_

#include "stm32f4xx_hal.h"

#include "utils/pin.h"
#include "utils/register.h"

/**
 * @struct ReceivedPacket
 * @brief Struct for storing the received packet data.
 *
 * This struct contains the status and data fields to hold the received packet information.
 */
struct ReceivedPacket
{
    uint8_t status; ///< Status byte of the received packet.
    uint8_t data; ///< Data byte of the received packet.
};

CREATE_REGISTER
(
	IC_Status_Register,
	ADDRESS(0x0)
	DEFAULT_VALUE(0x0)
	CREATE_FIELD(FAULT = { .addr = 0, .size = 1 })
	CREATE_FIELD(OT = { .addr = 1, .size = 1 })
	CREATE_FIELD(OVP = { .addr = 2, .size = 1 })
	CREATE_FIELD(NPOR = { .addr = 3, .size = 1 })
	CREATE_FIELD(OCP = { .addr = 4, .size = 1 })
	CREATE_FIELD(SPI_FLT = { .addr = 5, .size = 1 })
	CREATE_FIELD(BK_FLT = { .addr = 6, .size = 1 })
	CREATE_FIELD(MTR_LOCK = { .addr = 7, .size = 1 })

	uint8_t value;
)
CREATE_REGISTER
(
	Status_Register_1,
	ADDRESS(0x1)
	DEFAULT_VALUE(0x0)
	CREATE_FIELD(OCP_LA = { .addr = 0, .size = 1 })
	CREATE_FIELD(OCP_HA = { .addr = 1, .size = 1 })
	CREATE_FIELD(OCP_LB = { .addr = 2, .size = 1 })
	CREATE_FIELD(OCP_HB = { .addr = 3, .size = 1 })
	CREATE_FIELD(OCL_LC = { .addr = 4, .size = 1 })
	CREATE_FIELD(OCP_HC = { .addr = 5, .size = 1 })
	CREATE_FIELD(OTS = { .addr = 6, .size = 1 })
	CREATE_FIELD(OTW = { .addr = 7, .size = 1 })

	uint8_t value;
)
CREATE_REGISTER
(
	Status_Register_2,
	ADDRESS(0x2)
	DEFAULT_VALUE(0x0)
	CREATE_FIELD(SPI_ADDR_FLT = { .addr = 0, .size = 1 })
	CREATE_FIELD(SPI_SCLK_FLT = { .addr = 1, .size = 1 })
	CREATE_FIELD(SPI_PARITY = { .addr = 2, .size = 1 })
	CREATE_FIELD(VCP_UV = { .addr = 3, .size = 1 })
	CREATE_FIELD(BUCK_UV = { .addr = 4, .size = 1 })
	CREATE_FIELD(BUCK_OCP = { .addr = 5, .size = 1 })
	CREATE_FIELD(OTP_ERR = { .addr = 6, .size = 1 })

	uint8_t value;
)
CREATE_REGISTER
(
	Control_Register_1,
	ADDRESS(0x3)
	DEFAULT_VALUE(0x0)
	CREATE_FIELD(REG_LOCK = { .addr = 0, .size = 3 })

	uint8_t value;
)
CREATE_REGISTER
(
	Control_Register_2A,
	ADDRESS(0x4)
	DEFAULT_VALUE(0x80)
	CREATE_FIELD(CLR_FLT = { .addr = 0, .size = 1 })
	CREATE_FIELD(PWM_MODE = { .addr = 1, .size = 2 })
	CREATE_FIELD(SLEW = { .addr = 3, .size = 2 })
	CREATE_FIELD(SDO_MODE = { .addr = 5, .size = 1 })

	uint8_t value;
)
CREATE_REGISTER
(
	Control_Register_3,
	ADDRESS(0x5)
	DEFAULT_VALUE(0x46)
	CREATE_FIELD(OTW_REP = { .addr = 0, .size = 1 })
	CREATE_FIELD(OVP_EN = { .addr = 2, .size = 1 })
	CREATE_FIELD(OVP_SEL = { .addr = 3, .size = 1 })
	CREATE_FIELD(PWM_100_DUTY_SEL = { .addr = 4, .size = 1 })

	uint8_t value;
)
CREATE_REGISTER
(
	Control_Register_4,
	ADDRESS(0x6)
	DEFAULT_VALUE(0x10)
	CREATE_FIELD(OCP_MODE = { .addr = 0, .size = 2 })
	CREATE_FIELD(OCP_LVL = { .addr = 2, .size = 1 })
	CREATE_FIELD(OCP_RETRY = { .addr = 3, .size = 1 })
	CREATE_FIELD(OCP_DEG = { .addr = 4, .size = 2 })
	CREATE_FIELD(OCP_CBC = { .addr = 6, .size = 1 })
	CREATE_FIELD(DRV_OFF = { .addr = 7, .size = 1 })

	uint8_t value;
)
CREATE_REGISTER
(
	Control_Register_5,
	ADDRESS(0x7)
	DEFAULT_VALUE(0x0)
	CREATE_FIELD(CSA_GAIN = { .addr = 0, .size = 2 })
	CREATE_FIELD(EN_ASR = { .addr = 2, .size = 1 })
	CREATE_FIELD(EN_AAR = { .addr = 3, .size = 1 })
	CREATE_FIELD(ILIM_RECIR = { .addr = 6, .size = 1 })

	uint8_t value;
)
CREATE_REGISTER
(
	Control_Register_6,
	ADDRESS(0x8)
	DEFAULT_VALUE(0x0)
	CREATE_FIELD(BUCK_DIS = { .addr = 0, .size = 1 })
	CREATE_FIELD(BUCK_SEL = { .addr = 1, .size = 2 })
	CREATE_FIELD(BUCK_CL = { .addr = 3, .size = 1 })
	CREATE_FIELD(BUCK_PS_DIS = { .addr = 4, .size = 1 })

	uint8_t value;
)
CREATE_REGISTER
(
	Control_Register_7,
	ADDRESS(0x9)
	DEFAULT_VALUE(0x0)
	CREATE_FIELD(DIR = { .addr = 0, .size = 1 })
	CREATE_FIELD(BRAKE = { .addr = 1, .size = 1 })
	CREATE_FIELD(COAST = { .addr = 2, .size = 1 })
	CREATE_FIELD(BRAKE_MODE = { .addr = 3, .size = 1 })
	CREATE_FIELD(HALL_HYS = { .addr = 4, .size = 1 })

	uint8_t value;
)
CREATE_REGISTER
(
	Control_Register_8,
	ADDRESS(0xA)
	DEFAULT_VALUE(0x0)
	CREATE_FIELD(MTR_LOCK_MODE = { .addr = 0, .size = 2 })
	CREATE_FIELD(MTR_LOCK_TDET = { .addr = 2, .size = 2 })
	CREATE_FIELD(MTR_LOCK_RETRY = { .addr = 4, .size = 1 })
	CREATE_FIELD(FGOUT_SEL = { .addr = 6, .size = 2 })

	uint8_t value;
)
CREATE_REGISTER
(
	Control_Register_9,
	ADDRESS(0xB)
	DEFAULT_VALUE(0x0)
	CREATE_FIELD(ADVANCE_LVL = { .addr = 0, .size = 3 })

	uint8_t value;
)
CREATE_REGISTER
(
	Control_Register_10,
	ADDRESS(0xC)
	DEFAULT_VALUE(0x0)
	CREATE_FIELD(DLY_TARGET = { .addr = 0, .size = 4 })
	CREATE_FIELD(DLYCMP_EN = { .addr = 4, .size = 1 })

	uint8_t value;
)

/**
 * @brief Macro for adding a function that writes a value to a specified register field.
 *
 * This macro creates an inline function that sets the register field to the specified value
 * and sends the register value.
 *
 * @param functionName The name of the function to be created.
 * @param parameter The parameter type for the function.
 * @param valueToWrite The value to write to the register field.
 * @param registerField The register field to be written to.
 * @param registerValue The register to be modified.
 */
#define addFunction(functionName, parameter, valueToWrite, registerField, registerValue)		\
inline ReceivedPacket functionName(parameter)													\
{																								\
	setRegisterValue(registerField, valueToWrite, registerValue.value);							\
	return send(true, registerValue);															\
}

/**
 * @brief Macro for adding a function with no parameters that writes a value to a specified register field.
 *
 * This macro creates an inline function that sets the register field to the specified value
 * and sends the register value.
 *
 * @param functionName The name of the function to be created.
 * @param valueToWrite The value to write to the register field.
 * @param registerField The register field to be written to.
 * @param registerValue The register to be modified.
 */
#define addFunctionNoParam(functionName, valueToWrite, registerField, registerValue)			\
inline ReceivedPacket functionName()															\
{																								\
	setRegisterValue(registerField, valueToWrite, registerValue.value);							\
	return send(true, registerValue);															\
}

enum class OvervoltageSetting : uint8_t
{
	V22 = 1,
	V34 = 0,
};
enum class PwmFrequency : uint8_t
{
	KHz20 = 0,
	KHz40 = 1,
};
enum class PwmMode : uint8_t
{
	AsyncRectificationAnalogHall = 0,
	AsyncRectificationDigitalHall = 1,
	SyncRectificationAnalogHall = 2,
	SyncRectificationDigitalHall = 3,
};
enum class SlewRate : uint8_t
{
	V25PerMicrosecond = 0,
	V50PerMicrosecond = 1,
	V125PerMicrosecond = 2,
	V200PerMicrosecond = 3,
};
enum class SDOMode : uint8_t
{
	OpenDrain = 0,
	PushPull = 1,
};
enum class OCPDeglitchTime : uint8_t
{
	Microsecond0Dot2 = 0,
	Microsecond0Dot6 = 1,
	Microsecond1Dot25 = 2,
	Microsecond1Dot6 = 3,
};
enum class OCPRetryTime : uint8_t
{
	Millisecond5 = 0,
	Millisecond500 = 1,
};
enum class OvercurrentLevel : uint8_t
{
	A16 = 0,
	A24 = 1,
};
enum class OvercurrentFault : uint8_t
{
	CauseLatchedFault = 0,
	CauseAutomaticRetryingFault = 1,
	ReportOnly = 2,
	NoReportNoAction = 3,
};
enum class CurrentLimitRecirculation : uint8_t
{
	BrakeMode = 0,
	CoastMode = 1,
};
enum class CurrentSenseAmplifierGain : uint8_t
{
	mV150PerAmpere = 0,
	mV300PerAmpere = 1,
	mV600PerAmpere = 2,
	mV1200PerAmpere = 3,
};
enum class BuckCurrentLimit : uint8_t
{
	mA600 = 0,
	mA150 = 1,
};
enum class BuckVoltage : uint8_t
{
	V3Dot3 = 0,
	V4 = 2,
	V5 = 1,
	V5Dot7 = 3,
};

enum class HallComparatorHysteresis : uint8_t
{
	mV5 = 0,
	mV50 = 1,
};
enum class BrakeMode : uint8_t
{
	BrakeInBrakeMode = 0,
	CoastInBrakeMode = 1,
};
enum class MotorDirection : uint8_t
{
	CW = 0,
	CCW = 1,
};

enum class FGOUTFrequencyOverCommutationFrequency : uint8_t
{
	X3 = 0,
	X1 = 1,
	X0Dot5 = 2,
	X0Dot25 = 3,
};
enum class MotorLockRetryTime : uint8_t
{
	Millisecond500 = 0,
	Millisecond5000 = 1,
};
enum class MotorLockDetectionTime : uint8_t
{
	Millisecond300 = 0,
	Millisecond500 = 1,
	Millisecond1000 = 2,
	Millisecond5000 = 3,
};
enum class MotorLockFault : uint8_t
{
	CauseLatchedFault = 0,
	CauseAutomaticRetryingFault = 1,
	ReportOnly = 2,
	NoReportNoAction = 3,
};

enum class PhaseAdvance : uint8_t
{
	Degrees0 = 0,
	Degrees4 = 1,
	Degrees7 = 2,
	Degrees11 = 3,
	Degrees15 = 4,
	Degrees20 = 5,
	Degrees25 = 6,
	Degrees30 = 7,
};

enum class DelayTarget : uint8_t
{
	Microsecond0 = 0,
	Microsecond0Dot4 = 1,
	Microsecond0Dot6 = 2,
	Microsecond0Dot8 = 3,
	Microsecond1 = 4,
	Microsecond1Dot2 = 5,
	Microsecond1Dot4 = 6,
	Microsecond1Dot6 = 7,
	Microsecond1Dot8 = 8,
	Microsecond2 = 9,
	Microsecond2Dot2 = 0xA,
	Microsecond2Dot4 = 0xB,
	Microsecond2Dot6 = 0xC,
	Microsecond2Dot8 = 0xD,
	Microsecond3 = 0xE,
	Microsecond3Dot2 = 0xF,
};

/**
 * @class MCT8316ZSpi
 * @brief SPI interface class for the MCT8316Z motor controller.
 *
 * This class provides functions to communicate with the MCT8316Z motor controller via SPI.
 */
class MCT8316ZSpi {
public:
    /**
     * @brief Constructor for the MCT8316ZSpi class.
     *
     * Initializes the SPI interface with the given chip select pin and SPI handler.
     *
     * @param csPin Chip select pin for SPI communication.
     * @param spi SPI handler for communication.
     */
	MCT8316ZSpi(Pin csPin, SPI_HandleTypeDef* spi);
	virtual ~MCT8316ZSpi();

	addFunction(lockRegisterWrite, bool locked, locked ? 0x3 : 0x6, Control_Register_1::REG_LOCK, registerControl1)

	addFunctionNoParam(clearFault, 0x1, Control_Register_2A::CLR_FLT, registerControl2A)
	addFunction(setPwmMode, PwmMode setting, static_cast<uint8_t>(setting), Control_Register_2A::PWM_MODE, registerControl2A)
	addFunction(setSlewRate, SlewRate setting, static_cast<uint8_t>(setting), Control_Register_2A::SLEW, registerControl2A)
	addFunction(setSDOMode, SDOMode setting, static_cast<uint8_t>(setting), Control_Register_2A::SDO_MODE, registerControl2A)

	addFunction(enableOvervoltageProtection, bool enabled, enabled, Control_Register_3::OVP_EN, registerControl3)
	addFunction(enableOvertemperatureWarningReport, bool enabled, enabled, Control_Register_3::OTW_REP, registerControl3)
	addFunction(setOvervoltageSetting, OvervoltageSetting setting, static_cast<uint8_t>(setting), Control_Register_3::OVP_SEL, registerControl3)
	addFunction(setPwmFrequency, PwmFrequency setting, static_cast<uint8_t>(setting), Control_Register_3::PWM_100_DUTY_SEL, registerControl3)

	addFunction(enableLowPowerStandbyMode, bool enabled, enabled, Control_Register_4::DRV_OFF, registerControl4)
	addFunction(enableOCPClearingInPwmInputCycle, bool enabled, enabled, Control_Register_4::OCP_CBC, registerControl4)
	addFunction(setOCPDeglitchTime, OCPDeglitchTime setting, static_cast<uint8_t>(setting), Control_Register_4::OCP_DEG, registerControl4)
	addFunction(setOCPRetryTime, OCPRetryTime setting, static_cast<uint8_t>(setting), Control_Register_4::OCP_RETRY, registerControl4)
	addFunction(setOvercurrentLevel, OvercurrentLevel setting, static_cast<uint8_t>(setting), Control_Register_4::OCP_LVL, registerControl4)
	addFunction(setOvercurrentFaultOptions, OvercurrentFault setting, static_cast<uint8_t>(setting), Control_Register_4::OCP_MODE, registerControl4)

	addFunction(setCurrentLimitRecirculation, CurrentLimitRecirculation setting, static_cast<uint8_t>(setting), Control_Register_5::ILIM_RECIR, registerControl5)
	addFunction(enableActiveAsynshronousRectification, bool enabled, enabled, Control_Register_5::EN_AAR, registerControl5)
	addFunction(enableActiveSynshronousRectification, bool enabled, enabled, Control_Register_5::EN_ASR, registerControl5)
	addFunction(setCurrentSenseAmplifierGain, CurrentSenseAmplifierGain setting, static_cast<uint8_t>(setting), Control_Register_5::CSA_GAIN, registerControl5)

	addFunction(enableBuckPowerSequencing, bool enabled, !enabled, Control_Register_6::BUCK_PS_DIS, registerControl6)
	addFunction(setBuckCurrentLimit, BuckCurrentLimit setting, static_cast<uint8_t>(setting), Control_Register_6::BUCK_CL, registerControl6)
	addFunction(setBuckVoltage, BuckVoltage setting, static_cast<uint8_t>(setting), Control_Register_6::BUCK_SEL, registerControl6)
	addFunction(enableBuck, bool enabled, !enabled, Control_Register_6::BUCK_DIS, registerControl6)

	addFunction(setHallComparatorHysteresis, HallComparatorHysteresis setting, static_cast<uint8_t>(setting), Control_Register_7::HALL_HYS, registerControl7)
	addFunction(setBrakeModeSetting, BrakeMode setting, static_cast<uint8_t>(setting), Control_Register_7::BRAKE_MODE, registerControl7)
	addFunction(enableCoastMode, bool enabled, enabled, Control_Register_7::COAST, registerControl7)
	addFunction(enableBrakeMode, bool enabled, enabled, Control_Register_7::BRAKE, registerControl7)
	addFunction(setMotorDirection, MotorDirection setting, static_cast<uint8_t>(setting), Control_Register_7::DIR, registerControl7)

	addFunction(setFGOUTFrequencyOverCommutationFrequency, FGOUTFrequencyOverCommutationFrequency setting, static_cast<uint8_t>(setting), Control_Register_8::FGOUT_SEL, registerControl8)
	addFunction(setMotorLockRetryTime, MotorLockRetryTime setting, static_cast<uint8_t>(setting), Control_Register_8::MTR_LOCK_RETRY, registerControl8)
	addFunction(setMotorLockDetectionTime, MotorLockDetectionTime setting, static_cast<uint8_t>(setting), Control_Register_8::MTR_LOCK_TDET, registerControl8)
	addFunction(setMotorLockFaultOptions, MotorLockFault setting, static_cast<uint8_t>(setting), Control_Register_8::MTR_LOCK_MODE, registerControl8)

	addFunction(setPhaseAdvance, PhaseAdvance setting, static_cast<uint8_t>(setting), Control_Register_9::ADVANCE_LVL, registerControl9)

	addFunction(enableDriverDelayCompensation, bool enabled, enabled, Control_Register_10::DLYCMP_EN, registerControl10)
	// Check table 8-6 on the datasheet
	addFunction(setDelayTargetForDriverDelayCompensation, DelayTarget setting, static_cast<uint8_t>(setting), Control_Register_10::DLY_TARGET, registerControl10)

	/**
     * @brief Gets the FGOUT frequency over commutation frequency setting.
     *
     * This function reads the FGOUT frequency setting from the control register.
     *
     * @return FGOUT frequency setting.
     */
	inline FGOUTFrequencyOverCommutationFrequency getFGOUTFrequencyOverCommutationFrequencySetting() { return static_cast<FGOUTFrequencyOverCommutationFrequency>(getRegisterValue(Control_Register_8::FGOUT_SEL, registerControl8.value)); }
private:
    /**
     * @brief Sends a command via SPI.
     *
     * This function sends a command to the motor controller using SPI.
     *
     * @tparam T Register type.
     * @param write Whether the command is a write operation.
     * @param reg Register to be sent.
     * @return Received packet.
     */
	template <typename T>
	inline ReceivedPacket send(bool write, T reg)
	{
		return sendRaw(write, T::ADDRESS, reg.value);
	}
    /**
     * @brief Sends raw data via SPI.
     *
     * This function sends raw data to the motor controller using SPI.
     *
     * @param write Whether the command is a write operation.
     * @param address Address of the register.
     * @param data Data to be sent.
     * @return Received packet.
     */
	ReceivedPacket sendRaw(bool write, uint8_t address, uint8_t data);

private:
	Pin csPin; ///< Chip select pin for SPI communication.
	SPI_HandleTypeDef* spi; ///< SPI handler for communication.

	IC_Status_Register registerICStatus = IC_Status_Register { IC_Status_Register::DEFAULT };
	Status_Register_1 registerStatus1 = Status_Register_1 { Status_Register_1::DEFAULT };
	Status_Register_2 registerStatus2 = Status_Register_2 { Status_Register_2::DEFAULT };
	Control_Register_1 registerControl1 = Control_Register_1 { Control_Register_1::DEFAULT };
	Control_Register_2A registerControl2A = Control_Register_2A { Control_Register_2A::DEFAULT };
	Control_Register_3 registerControl3 = Control_Register_3 { Control_Register_3::DEFAULT };
	Control_Register_4 registerControl4 = Control_Register_4 { Control_Register_4::DEFAULT };
	Control_Register_5 registerControl5 = Control_Register_5 { Control_Register_5::DEFAULT };
	Control_Register_6 registerControl6 = Control_Register_6 { Control_Register_6::DEFAULT };
	Control_Register_7 registerControl7 = Control_Register_7 { Control_Register_7::DEFAULT };
	Control_Register_8 registerControl8 = Control_Register_8 { Control_Register_8::DEFAULT };
	Control_Register_9 registerControl9 = Control_Register_9 { Control_Register_9::DEFAULT };
	Control_Register_10 registerControl10 = Control_Register_10 { Control_Register_10::DEFAULT };
};

#endif /* MCT8316Z_H_ */
