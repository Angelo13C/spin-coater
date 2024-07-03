/*
 * display.cpp
 *
 *  Created on: Jun 26, 2024
 *      Author: Angelo
 */

#include <spin_coater/display.h>
#include <cstdio>

uint16_t Display::buffer1[Display::BUFFER_SIZE];
uint16_t Display::buffer2[Display::BUFFER_SIZE];
ILI9341 Display::ili9341;
XPT2046 Display::xpt2046;

Display::Display(ILI9341 ili9341, XPT2046 xpt2046)
{
	Display::ili9341 = ili9341;
	Display::xpt2046 = xpt2046;

	lv_init();
	lv_display_t* display = lv_display_create(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	lv_display_set_flush_cb(display, Display::flushDisplay); /*Set a flush callback to draw to the display*/
	lv_display_set_buffers(display, buffer1, buffer2, sizeof(buffer1), LV_DISPLAY_RENDER_MODE_PARTIAL); /*Set an initialized buffer*/

	lv_indev_t* touchDevice = lv_indev_create();
	lv_indev_set_type(touchDevice, LV_INDEV_TYPE_POINTER);
	lv_indev_set_read_cb(touchDevice, Display::readTouchPosition);

	createUI();
}

Display::~Display()
{
	lv_obj_clean(lv_screen_active());
}

static void setObjectEnabled(lv_obj_t* object, bool enabled)
{
	auto flagToAdd = enabled ? LV_OBJ_FLAG_CLICKABLE : LV_OBJ_FLAG_HIDDEN;
	auto flagToRemove = enabled ? LV_OBJ_FLAG_HIDDEN : LV_OBJ_FLAG_CLICKABLE;
    lv_obj_add_flag(object, flagToAdd);
    lv_obj_remove_flag(object, flagToRemove);
}

void Display::flushDisplay(lv_display_t *display, const lv_area_t *area, uint8_t *colorP)
{
	Display::ili9341.set_window((uint16_t) area->x1, (uint16_t) area->y1, (uint16_t) area->x2, (uint16_t) area->y2);

	int32_t height = area->y2 - area->y1 + 1;
	int32_t width = area->x2 - area->x1 + 1;
	Display::ili9341.send_colors(colorP, (size_t) (width * height));

	/* IMPORTANT!!!
	* Inform the graphics library that you are ready with the flushing*/
	lv_display_flush_ready(display);
}

void Display::readTouchPosition(lv_indev_t* indev, lv_indev_data_t* data)
{
	uint16_t x, y;
	if(Display::xpt2046.readCurrentTouchPosition<15>(x, y))
	{
	    data->state = LV_INDEV_STATE_PRESSED;
	    data->point.x = x;
	    data->point.y = y;
	}
	else
		data->state = LV_INDEV_STATE_RELEASED;
}

void Display::configure(ProcessPoints *points, std::function<void()> onStartButtonPressedFn, std::function<void()> onStopButtonPressedFn, uint16_t** keypadNumber)
{
	this->points = points;

	this->onStartButtonPressedFn = [this, onStartButtonPressedFn] {
		(onStartButtonPressedFn)();

	    setObjectEnabled(this->startButton, false);
	    setObjectEnabled(this->stopButton, true);
	};
	this->onStopButtonPressedFn = [this, onStopButtonPressedFn] {
		(onStopButtonPressedFn)();

	    setObjectEnabled(this->startButton, true);
	    setObjectEnabled(this->stopButton, false);
	};

	for(size_t i = 0; i < POINTS_COUNT; i++)
	{
		this->onStartEditingPointFn[i] = [this, i] {
			this->pointCurrentlyBeingEdited = (uint8_t) i;

			this->refreshLabel();

		    setObjectEnabled(this->homeUI, false);
		    setObjectEnabled(this->editPointUI, true);
		};
	}
	this->onFinishEditingPointFn = [this, &keypadNumber] {
		this->pointCurrentlyBeingEdited = NO_POINT_BEING_EDITED;
		*keypadNumber = nullptr;

	    setObjectEnabled(this->homeUI, true);
	    setObjectEnabled(this->editPointUI, false);
	};

	this->onStartEditingPointSpeedFn = [this, &keypadNumber] {
		*keypadNumber = &this->points->points[this->pointCurrentlyBeingEdited].speedInRpm;
	};
	this->onStartEditingPointDurationFn = [this, &keypadNumber] {
		*keypadNumber = &this->points->points[this->pointCurrentlyBeingEdited].durationInSeconds;
	};
}

static void onButtonPressed(lv_event_t *event)
{
	auto function = static_cast<std::function<void()>*>(event->user_data);
	(*function)();
}

static constexpr int32_t ELEMENT_WIDTH = 60;
static constexpr int32_t ELEMENT_HEIGHT = 140;

static uint16_t calculateElementPosition(uint8_t slot)
{
	return 15 + slot * (ELEMENT_WIDTH + 15);
}

void Display::refreshLabel()
{
	if(points != nullptr)
	{
		auto point = points->points[pointCurrentlyBeingEdited];
		lv_label_set_text_fmt(speedLabelEditPointUI, "%d rpm", point.speedInRpm);
		lv_label_set_text_fmt(durationLabelEditPointUI, "%d seconds", point.durationInSeconds);
		lv_label_set_text_fmt(speedLabelsHomeUI[pointCurrentlyBeingEdited], "%d rpm", point.speedInRpm);
		lv_label_set_text_fmt(durationLabelsHomeUI[pointCurrentlyBeingEdited], "%d seconds", point.durationInSeconds);

		if(points->usedPointsCount != POINTS_COUNT)
		{
			setObjectEnabled(newPointButton, true);
			lv_obj_set_x(newPointButton, calculateElementPosition(points->usedPointsCount));
		}
		else
			setObjectEnabled(newPointButton, false);
	}
}

void Display::createUI()
{
	lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);

	createHomeUI();
	createEditPointUI();
	setObjectEnabled(editPointUI, false);

	lv_obj_t* creditsLabel = lv_label_create(lv_screen_active());
	lv_label_set_long_mode(creditsLabel, LV_LABEL_LONG_WRAP);
	lv_label_set_text_static(creditsLabel, "ANGELO CIPRIANI | Spin coater");
	lv_obj_set_width(creditsLabel, DISPLAY_WIDTH);
	lv_obj_set_style_text_align(creditsLabel, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_align(creditsLabel, LV_ALIGN_OUT_TOP_MID, 0, -30);
}

void Display::createHomeUI()
{
	homeUI = lv_obj_create(lv_screen_active());

	const int32_t TOP_AREA_HEIGHT = 160;
	lv_obj_t* topArea = lv_obj_create(homeUI);
    lv_obj_set_size(topArea, DISPLAY_WIDTH, TOP_AREA_HEIGHT);
	lv_obj_align(topArea, LV_ALIGN_OUT_TOP_MID, 0, 0);

	for(uint8_t i = 0; i < points->usedPointsCount; i++)
	{
		lv_obj_t* element = lv_obj_create(topArea);
	    lv_obj_set_size(element, ELEMENT_WIDTH, ELEMENT_HEIGHT);
	    lv_obj_align(element, LV_ALIGN_TOP_LEFT, calculateElementPosition(i), -40);

		auto point = points->points[i];

		const int32_t LABEL_WIDTH = ELEMENT_WIDTH - 10;
		lv_obj_t* speedLabel = lv_label_create(element);
		lv_label_set_long_mode(speedLabel, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
		lv_label_set_text_fmt(speedLabel, "%d rpm", point.speedInRpm);
		lv_obj_set_width(speedLabel, LABEL_WIDTH);
		lv_obj_set_style_text_align(speedLabel, LV_TEXT_ALIGN_RIGHT, 0);
		lv_obj_align(speedLabel, LV_ALIGN_OUT_TOP_MID, 0, -20);
		speedLabelsHomeUI[i] = speedLabel;

		lv_obj_t* durationLabel = lv_label_create(element);
		lv_label_set_long_mode(durationLabel, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
		lv_label_set_text_fmt(durationLabel, "%d seconds", point.durationInSeconds);
		lv_obj_set_width(durationLabel, LABEL_WIDTH);
		lv_obj_set_style_text_align(durationLabel, LV_TEXT_ALIGN_RIGHT, 0);
		lv_obj_align(durationLabel, LV_ALIGN_OUT_TOP_MID, 0, -60);
		durationLabelsHomeUI[i] = durationLabel;

	    lv_obj_t* editButton = lv_button_create(element);
	    lv_obj_add_event_cb(editButton, onButtonPressed, LV_EVENT_CLICKED, static_cast<void*>(&onStartEditingPointFn[i]));
	    lv_obj_align(editButton, LV_ALIGN_BOTTOM_MID, 0, 0);
	    lv_obj_remove_flag(editButton, LV_OBJ_FLAG_PRESS_LOCK);
	    lv_obj_t* editButtonLabel = lv_label_create(editButton);
	    lv_label_set_text_static(editButtonLabel, "EDIT");
	    lv_obj_center(editButtonLabel);
	}

	newPointButton = lv_button_create(topArea);
    lv_obj_add_event_cb(newPointButton, onButtonPressed, LV_EVENT_CLICKED, static_cast<void*>(&onStartButtonPressedFn));
    lv_obj_align(newPointButton, LV_ALIGN_CENTER, 0, 0);
    lv_obj_remove_flag(newPointButton, LV_OBJ_FLAG_PRESS_LOCK);
    lv_obj_t* newPointButtonLabel = lv_label_create(newPointButton);
    lv_label_set_text_static(newPointButtonLabel, "+");
    lv_obj_center(newPointButtonLabel);

	lv_obj_t* bottomArea = lv_obj_create(homeUI);
    lv_obj_set_size(bottomArea, DISPLAY_WIDTH, DISPLAY_HEIGHT - TOP_AREA_HEIGHT);
	lv_obj_align(bottomArea, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    startButton = lv_button_create(bottomArea);
    lv_obj_add_event_cb(startButton, onButtonPressed, LV_EVENT_CLICKED, static_cast<void*>(&onStartButtonPressedFn));
    lv_obj_align(startButton, LV_ALIGN_CENTER, 0, 0);
    lv_obj_remove_flag(startButton, LV_OBJ_FLAG_PRESS_LOCK);
    lv_obj_t* startButtonLabel = lv_label_create(startButton);
    lv_label_set_text_static(startButtonLabel, "START");
    lv_obj_center(startButtonLabel);

    stopButton = lv_button_create(bottomArea);
    lv_obj_add_event_cb(stopButton, onButtonPressed, LV_EVENT_CLICKED, static_cast<void*>(&onStopButtonPressedFn));
    lv_obj_align(stopButton, LV_ALIGN_CENTER, 0, 0);
    lv_obj_remove_flag(stopButton, LV_OBJ_FLAG_PRESS_LOCK);
    lv_obj_t* stopButtonLabel = lv_label_create(stopButton);
    lv_label_set_text_static(stopButtonLabel, "STOP");
    lv_obj_center(stopButtonLabel);
    setObjectEnabled(stopButton, false);
}

void Display::createEditPointUI()
{
	editPointUI = lv_obj_create(lv_screen_active());

	const int32_t TOP_AREA_HEIGHT = 160;
	lv_obj_t* topArea = lv_obj_create(editPointUI);
    lv_obj_set_size(topArea, DISPLAY_WIDTH, TOP_AREA_HEIGHT);
	lv_obj_align(topArea, LV_ALIGN_OUT_TOP_MID, 0, 0);

	const int32_t ELEMENT_HEIGHT = TOP_AREA_HEIGHT - 20;
	lv_obj_t* element = lv_obj_create(topArea);
	lv_obj_set_size(element, DISPLAY_WIDTH, ELEMENT_HEIGHT);
	lv_obj_align(element, LV_ALIGN_OUT_TOP_MID, 0, -40);

	const int32_t LABEL_WIDTH = DISPLAY_WIDTH - 40;
	speedLabelEditPointUI = lv_label_create(element);
	lv_label_set_long_mode(speedLabelEditPointUI, LV_LABEL_LONG_WRAP);
	lv_label_set_text_static(speedLabelEditPointUI, "0 rpm");
	lv_obj_set_width(speedLabelEditPointUI, LABEL_WIDTH);
	lv_obj_set_style_text_align(speedLabelEditPointUI, LV_TEXT_ALIGN_RIGHT, 0);
	lv_obj_align(speedLabelEditPointUI, LV_ALIGN_OUT_TOP_MID, 0, -20);

	durationLabelEditPointUI = lv_label_create(element);
	lv_label_set_long_mode(durationLabelEditPointUI, LV_LABEL_LONG_WRAP);
	lv_label_set_text_static(durationLabelEditPointUI, "0 seconds");
	lv_obj_set_width(durationLabelEditPointUI, LABEL_WIDTH);
	lv_obj_set_style_text_align(durationLabelEditPointUI, LV_TEXT_ALIGN_RIGHT, 0);
	lv_obj_align(durationLabelEditPointUI, LV_ALIGN_OUT_TOP_MID, 0, -60);

	lv_obj_t* bottomArea = lv_obj_create(editPointUI);
    lv_obj_set_size(bottomArea, DISPLAY_WIDTH, DISPLAY_HEIGHT - TOP_AREA_HEIGHT);
	lv_obj_align(bottomArea, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

	lv_obj_t* finishButton = lv_button_create(bottomArea);
    lv_obj_add_event_cb(finishButton, onButtonPressed, LV_EVENT_CLICKED, static_cast<void*>(&onStartButtonPressedFn));
    lv_obj_align(finishButton, LV_ALIGN_CENTER, 0, 0);
    lv_obj_remove_flag(finishButton, LV_OBJ_FLAG_PRESS_LOCK);
    lv_obj_t* finishButtonLabel = lv_label_create(finishButton);
    lv_label_set_text_static(finishButtonLabel, "FINISH");
    lv_obj_center(finishButtonLabel);
}
