/*
 * register.h
 *
 *  Created on: Jun 15, 2024
 *      Author: Angelo
 */

#ifndef INC_REGISTER_H_
#define INC_REGISTER_H_

// Macro to create a register struct with the specified name and fields
#define CREATE_REGISTER(name, ...) struct name { __VA_ARGS__ };
// Macro to create a static constexpr RegisterField with the specified attributes
#define CREATE_FIELD(...) static constexpr RegisterField __VA_ARGS__;
// Macro to set a default value for a register
#define DEFAULT_VALUE(defaultValue) static constexpr uint8_t DEFAULT = defaultValue;
// Macro to set the address of a register
#define ADDRESS(address) static constexpr uint8_t ADDRESS = address;

/**
 * @struct RegisterField
 * @brief Struct to represent a field within a register.
 */
struct RegisterField
{
	uint8_t addr;     ///< The address of the field within the register.
	uint8_t size = 1; ///< The size of the field in bits. Default is 1 bit.
};

/**
 * @brief Set the value of a specific field within a register.
 * @param field The field to set.
 * @param fieldValue The value to set the field to.
 * @param registerValue The current value of the register, which will be modified.
 */
inline void setRegisterValue(RegisterField field, uint8_t fieldValue, uint8_t& registerValue)
{
	auto mask = ((1 << field.size) - 1) << field.addr;
	registerValue &= ~mask;
	registerValue |= fieldValue << field.addr;
}
/**
 * @brief Get the value of a specific field from a register.
 * @param field The field to get the value from.
 * @param value The current value of the register.
 * @return The value of the field.
 */
inline int getRegisterValue(RegisterField field, uint8_t value)
{
	if(field.size == 8)
		return value;

	auto mask = ((1 << field.size) - 1) << field.addr;
	value &= mask;
	return value >> field.addr;
}


#endif /* INC_REGISTER_H_ */
