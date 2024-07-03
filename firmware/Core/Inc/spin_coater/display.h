/*
 * display.h
 *
 *  Created on: Jun 26, 2024
 *      Author: Angelo
 */

#ifndef INC_SPIN_COATER_DISPLAY_H_
#define INC_SPIN_COATER_DISPLAY_H_

#include "lvgl/lvgl.h"
#include "drivers/ili9341/driver.h"
#include "drivers/xpt2046/driver.h"
#include "spin_coating_process.h"
#include <functional>

/**
 * @brief Display class handles the user interface on ILI9341 display using LVGL.
 */
class Display {
public:
    /**
     * @brief Constructs a new Display object.
     *
     * You must call configure on the Display object after creating it!
     *
     * @param ili9341 Reference to the ILI9341 driver object.
     */
	Display(ILI9341 ili9341, XPT2046 xpt2046);
	virtual ~Display();

    /**
     * @brief Configures the display with process points and button callbacks.
     *
     * @param points Pointer to the ProcessPoints object.
     * @param onStartButtonPressedFn Callback function for start button press event.
     * @param onStopButtonPressedFn Callback function for stop button press event.
     * @param keypadNumber Pointer to the keypad number array.
     */
	void configure(ProcessPoints* points, std::function<void()> onStartButtonPressedFn, std::function<void()> onStopButtonPressedFn, uint16_t** keypadNumber);

	/**
     * @brief Handles LVGL timer tick.
     */
	inline void tick() { lv_timer_handler(); }

    /**
     * @brief Refreshes the labels on the display (due to some of them changing).
     */
	void refreshLabel();

private:
	static void flushDisplay(lv_display_t* display, const lv_area_t* area, uint8_t* colorP);
	static void readTouchPosition(lv_indev_t* indev, lv_indev_data_t* data);

	void createUI();
	void createHomeUI();
	void createEditPointUI();

private:
	static ILI9341 ili9341;
	static XPT2046 xpt2046;

	static constexpr uint32_t DISPLAY_WIDTH = 320;
	static constexpr uint32_t DISPLAY_HEIGHT = 240;

	static constexpr uint32_t BUFFER_SIZE = DISPLAY_WIDTH * 20;
	static uint16_t buffer1[BUFFER_SIZE]; //TODO: Chose a buffer size.
	static uint16_t buffer2[BUFFER_SIZE];

	ProcessPoints* points = nullptr;
	static constexpr uint8_t NO_POINT_BEING_EDITED = 255;
	uint8_t pointCurrentlyBeingEdited = NO_POINT_BEING_EDITED;

	std::function<void()> onStartButtonPressedFn;
	std::function<void()> onStopButtonPressedFn;
	std::function<void()> onStartEditingPointFn[POINTS_COUNT];
	std::function<void()> onStartEditingPointSpeedFn;
	std::function<void()> onStartEditingPointDurationFn;
	std::function<void()> onFinishEditingPointFn;

	lv_obj_t* homeUI = nullptr;
	lv_obj_t* editPointUI = nullptr;

	lv_obj_t* startButton = nullptr;
	lv_obj_t* stopButton = nullptr;
	lv_obj_t* newPointButton = nullptr;

	lv_obj_t* speedLabelsHomeUI[POINTS_COUNT] = {};
	lv_obj_t* durationLabelsHomeUI[POINTS_COUNT] = {};
	lv_obj_t* speedLabelEditPointUI = nullptr;
	lv_obj_t* durationLabelEditPointUI = nullptr;
};

#endif /* INC_SPIN_COATER_DISPLAY_H_ */
