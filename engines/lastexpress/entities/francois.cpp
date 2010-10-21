/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include "lastexpress/entities/francois.h"

#include "lastexpress/game/action.h"
#include "lastexpress/game/entities.h"
#include "lastexpress/game/inventory.h"
#include "lastexpress/game/logic.h"
#include "lastexpress/game/object.h"
#include "lastexpress/game/savepoint.h"
#include "lastexpress/game/state.h"

#include "lastexpress/helpers.h"
#include "lastexpress/lastexpress.h"

namespace LastExpress {

Francois::Francois(LastExpressEngine *engine) : Entity(engine, kEntityFrancois) {
	ADD_CALLBACK_FUNCTION(Francois, reset);
	ADD_CALLBACK_FUNCTION(Francois, updateFromTime);
	ADD_CALLBACK_FUNCTION(Francois, draw);
	ADD_CALLBACK_FUNCTION(Francois, enterExitCompartment);
	ADD_CALLBACK_FUNCTION(Francois, enterExitCompartment2);
	ADD_CALLBACK_FUNCTION(Francois, playSound);
	ADD_CALLBACK_FUNCTION(Francois, savegame);
	ADD_CALLBACK_FUNCTION(Francois, updateEntity);
	ADD_CALLBACK_FUNCTION(Francois, function9);
	ADD_CALLBACK_FUNCTION(Francois, function10);
	ADD_CALLBACK_FUNCTION(Francois, function11);
	ADD_CALLBACK_FUNCTION(Francois, function12);
	ADD_CALLBACK_FUNCTION(Francois, function13);
	ADD_CALLBACK_FUNCTION(Francois, function14);
	ADD_CALLBACK_FUNCTION(Francois, function15);
	ADD_CALLBACK_FUNCTION(Francois, function16);
	ADD_CALLBACK_FUNCTION(Francois, chapter1);
	ADD_CALLBACK_FUNCTION(Francois, chapter1Handler);
	ADD_CALLBACK_FUNCTION(Francois, function19);
	ADD_CALLBACK_FUNCTION(Francois, function20);
	ADD_CALLBACK_FUNCTION(Francois, chapter2);
	ADD_CALLBACK_FUNCTION(Francois, chapter2Handler);
	ADD_CALLBACK_FUNCTION(Francois, function23);
	ADD_CALLBACK_FUNCTION(Francois, chapter3);
	ADD_CALLBACK_FUNCTION(Francois, chapter3Handler);
	ADD_CALLBACK_FUNCTION(Francois, chapter4);
	ADD_CALLBACK_FUNCTION(Francois, chapter4Handler);
	ADD_CALLBACK_FUNCTION(Francois, chapter5);
	ADD_CALLBACK_FUNCTION(Francois, chapter5Handler);
	ADD_CALLBACK_FUNCTION(Francois, function30);
	ADD_NULL_FUNCTION();
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(1, Francois, reset)
	Entity::reset(savepoint, true);
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION_I(2, Francois, updateFromTime, uint32)
	Entity::updateFromTime(savepoint);
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION_S(3, Francois, draw)
	Entity::draw(savepoint);
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION_SI(4, Francois, enterExitCompartment, ObjectIndex)
	Entity::enterExitCompartment(savepoint);
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION_SI(5, Francois, enterExitCompartment2, ObjectIndex)
	Entity::enterExitCompartment(savepoint, kPosition_5790, kPosition_6130, kCarRedSleeping, kObjectCompartmentD, true);
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION_S(6, Francois, playSound)
	Entity::playSound(savepoint);
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION_II(7, Francois, savegame, SavegameType, uint32)
	Entity::savegame(savepoint);
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION_II(8, Francois, updateEntity, CarIndex, EntityPosition)
	error("Francois: callback function 8 not implemented!");
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(9, Francois, function9)
	switch (savepoint.action) {
	default:
		break;

	case kActionDefault:
		if (getObjects()->get(kObjectCompartmentD).location == kObjectLocation2) {
			getObjects()->update(kObjectCompartmentD, kEntityPlayer, kObjectLocationNone, kCursorKeepValue, kCursorKeepValue);
			getSavePoints()->push(kEntityFrancois, kEntityMmeBoutarel, kAction134289824);
			setCallback(1);
			setup_enterExitCompartment("605Cd", kObjectCompartmentD);
		} else {
			setCallback(2);
			setup_enterExitCompartment("605Ed", kObjectCompartmentD);
		}
		break;

	case kActionCallback:
		switch (getCallback()) {
		default:
			break;

		case 1:
			getObjects()->update(kObjectCompartmentD, kEntityPlayer, kObjectLocation2, kCursorKeepValue, kCursorKeepValue);
			break;

		case 2:
			getData()->location = kLocationOutsideCompartment;
			CALLBACK_ACTION();
			break;
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(10, Francois, function10)
	switch (savepoint.action) {
	default:
		break;

	case kActionDefault:
		if (getObjects()->get(kObjectCompartmentD).location == kObjectLocation2) {
			getObjects()->update(kObjectCompartmentD, kEntityPlayer, kObjectLocationNone, kCursorKeepValue, kCursorKeepValue);
			setCallback(1);
			setup_enterExitCompartment("605Bd", kObjectCompartmentD);
		} else {
			setCallback(2);
			setup_enterExitCompartment("605Dd", kObjectCompartmentD);
		}
		break;

	case kActionCallback:
		switch (getCallback()) {
		default:
			break;

		case 1:
			getObjects()->update(kObjectCompartmentD, kEntityPlayer, kObjectLocation2, kCursorKeepValue, kCursorKeepValue);
			getSavePoints()->push(kEntityFrancois, kEntityMmeBoutarel, kAction102484312);
			break;

		case 2:
			getData()->location = kLocationInsideCompartment;
			getEntities()->clearSequences(kEntityFrancois);

			CALLBACK_ACTION();
			break;
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION_I(11, Francois, function11, TimeValue)
	switch (savepoint.action) {
	default:
		break;

	case kActionNone:
		if (!getSound()->isBuffered(kEntityFrancois)) {

			UPDATE_PARAM_PROC(CURRENT_PARAMS(1, 1), getState()->timeTicks, params->param6)
				switch (rnd(7)) {
				default:
					break;

				case 0:
					getSound()->playSound(kEntityFrancois, "Fra1002A");
					break;

				case 1:
					getSound()->playSound(kEntityFrancois, "Fra1002B");
					break;

				case 2:
					getSound()->playSound(kEntityFrancois, "Fra1002C");
					break;

				case 3:
					getSound()->playSound(kEntityFrancois, "Fra1002D");
					break;

				case 4:
					getSound()->playSound(kEntityFrancois, "Fra1002E");
					break;

				case 5:
				case 6:
					getSound()->playSound(kEntityFrancois, "Fra1002F");
					break;
				}

				params->param6 = 15 * rnd(7);
				CURRENT_PARAMS(1, 1) = 0;
			UPDATE_PARAM_PROC_END
		}

		if (!getEntities()->hasValidFrame(kEntityFrancois) || !getEntities()->isWalkingOppositeToPlayer(kEntityFrancois))
			getData()->inventoryItem = kItemNone;

		if (getEntities()->updateEntity(kEntityFrancois, (CarIndex)params->param2, (EntityPosition)params->param3)) {
			params->param5 = 0;

			if (params->param3 == kPosition_540) {
				params->param2 = (getProgress().chapter == kChapter1) ? kCarRedSleeping : kCarGreenSleeping;
				params->param3 = kPosition_9460;
			} else {
				params->param2 = kCarGreenSleeping;
				params->param3 = kPosition_540;
				params->param7 = 0;
				params->param8 = 0;

				getSavePoints()->push(kEntityFrancois, kEntityCoudert, kAction225932896);
				getSavePoints()->push(kEntityFrancois, kEntityMertens, kAction225932896);
			}
		}

		if (getEntities()->checkDistanceFromPosition(kEntityFrancois, kPosition_2000, 500) && getData()->direction == kDirectionDown) {

			if (getEntities()->isInsideTrainCar(kEntityFrancois, kCarRedSleeping) && params->param8) {
				setCallback(2);
				setup_draw("605A");
				break;
			}

			if (getEntities()->isInsideTrainCar(kEntityFrancois, kCarGreenSleeping) && params->param7) {
				setCallback(3);
				setup_draw("605A");
				break;
			}
		}

label_callback:
		if (getProgress().chapter == kChapter1) {

			if (getEntities()->isInsideTrainCar(kEntityFrancois, kCarRedSleeping)
			 && (getEntities()->hasValidFrame(kEntityFrancois) || params->param1 < getState()->time || params->param4)
			 && !params->param5
			 && getData()->entityPosition < getEntityData(kEntityMmeBoutarel)->entityPosition) {

				if (getData()->direction == kDirectionDown) {
					getSavePoints()->push(kEntityFrancois, kEntityMmeBoutarel, kAction202221040);
					params->param4 = 1;
					params->param5 = 1;
				} else if (params->param4 && getEntities()->isDistanceBetweenEntities(kEntityFrancois, kEntityMmeBoutarel, 1000)) {
					getSavePoints()->push(kEntityFrancois, kEntityMmeBoutarel, kAction168986720);
					params->param5 = 1;
				}
			}
		} else if (params->param1 < getState()->time) {
			getData()->clothes = kClothesDefault;
			getData()->field_4A3 = 30;
			getData()->inventoryItem = kItemNone;

			if (getSound()->isBuffered(kEntityFrancois))
				getSound()->processEntry(kEntityFrancois);

			setCallback(4);
			setup_updateEntity(kCarRedSleeping, kPosition_5790);
		}
		break;

	case kAction1:
		getData()->inventoryItem = kItemNone;

		if (getSound()->isBuffered(kEntityFrancois))
			getSound()->processEntry(kEntityFrancois);

		setCallback(6);
		setup_savegame(kSavegameTypeEvent, kEventFrancoisWhistle);
		break;

	case kActionExcuseMeCath:
		if (getProgress().jacket == kJacketGreen
		 && !getEvent(kEventFrancoisWhistle)
		 && !getEvent(kEventFrancoisWhistleD)
		 && !getEvent(kEventFrancoisWhistleNight)
		 && !getEvent(kEventFrancoisWhistleNightD))
			getData()->inventoryItem = kItemInvalid;
		break;

	case kActionDefault:
		setCallback(1);
		setup_function9();
		break;

	case kActionCallback:
		switch (getCallback()) {
		default:
			break;

		case 1:
			getData()->clothes = kClothes1;
			getData()->field_4A3 = 100;
			getData()->inventoryItem = kItemNone;

			params->param2 = kCarGreenSleeping;
			params->param3 = kPosition_540;

			getEntities()->updateEntity(kEntityFrancois, kCarGreenSleeping, kPosition_540);

			params->param6 = 15 * rnd(7);
			break;

		case 2:
			getSavePoints()->push(kEntityFrancois, kEntityCoudert, kAction168253822);
			// Fallback to next case

		case 3:
			params->param2 = kCarRedSleeping;
			params->param3 = kPosition_9460;
			params->param5 = 0;

			getData()->entityPosition = kPosition_2088;

			getEntities()->updateEntity(kEntityFrancois, kCarRedSleeping, kPosition_9460);
			goto label_callback;

		case 4:
			setCallback(5);
			setup_function10();
			break;

		case 5:
			CALLBACK_ACTION();
			break;

		case 6:
			if (getProgress().jacket == kJacketGreen) {
				if (isNight())
					getAction()->playAnimation(getData()->entityPosition <= getEntityData(kEntityPlayer)->entityPosition ? kEventFrancoisWhistleNightD : kEventFrancoisWhistleNight);
				else
					getAction()->playAnimation(getData()->entityPosition <= getEntityData(kEntityPlayer)->entityPosition ? kEventFrancoisWhistleD : kEventFrancoisWhistleD);
			}
			getEntities()->loadSceneFromEntityPosition(getData()->car, (EntityPosition)(getData()->entityPosition + 750 * (getData()->direction == kDirectionUp ? -1 : 1)), getData()->direction == kDirectionUp);
			break;
		}
		break;

	case kAction102752636:
		getEntities()->clearSequences(kEntityFrancois);
		getData()->location = kLocationInsideCompartment;
		getData()->entityPosition = kPosition_5790;
		getData()->clothes = kClothesDefault;
		getData()->field_4A3 = 30;
		getData()->inventoryItem = kItemNone;

		CALLBACK_ACTION();
		break;

	case kAction205346192:
		if (savepoint.entity2 == kEntityCoudert)
			params->param8 = 1;
		else if (savepoint.entity2 == kEntityMertens)
			params->param7 = 1;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(12, Francois, function12)
	switch (savepoint.action) {
	default:
		break;

	case kActionDefault:
		setCallback(1);
		setup_function9();
		break;

	case kActionCallback:
		switch (getCallback()) {
		default:
			break;

		case 1:
			setCallback(2);
			setup_updateEntity(kCarRedSleeping, kPosition_9460);
			break;

		case 2:
			setCallback(3);
			setup_updateFromTime(675);
			break;

		case 3:
			setCallback(4);
			setup_updateEntity(kCarRedSleeping, kPosition_540);
			break;

		case 4:
			setCallback(5);
			setup_updateFromTime(675);
			break;

		case 5:
			setCallback(6);
			setup_updateEntity(kCarRedSleeping, kPosition_5790);
			break;

		case 6:
			setCallback(7);
			setup_function10();
			break;

		case 7:
			CALLBACK_ACTION();
			break;
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(13, Francois, function13)
	switch (savepoint.action) {
	default:
		break;

	case kActionDefault:
		setCallback(1);
		setup_function9();
		break;

	case kActionCallback:
		switch (getCallback()) {
		default:
			break;

		case 1:
			setCallback(2);
			setup_updateEntity(kCarRedSleeping, kPosition_540);
			break;

		case 2:
			setCallback(3);
			setup_updateEntity(kCarGreenSleeping, kPosition_4070);
			break;

		case 3:
			setCallback(4);
			setup_enterExitCompartment("605Df", kObjectCompartment6);
			break;

		case 4:
			getData()->location = kLocationInsideCompartment;
			getEntities()->clearSequences(kEntityFrancois);

			setCallback(5);
			setup_playSound("Har2010");
			break;

		case 5:
			getSavePoints()->push(kEntityFrancois, kEntityAlouan, kAction189489753);
			break;

		case 6:
			getData()->location = kLocationOutsideCompartment;

			setCallback(7);
			setup_updateEntity(kCarGreenSleeping, kPosition_4840);
			break;

		case 7:
			if (getInventory()->hasItem(kItemWhistle) || getInventory()->get(kItemWhistle)->location == kObjectLocation3) {
				setCallback(10);
				setup_updateEntity(kCarGreenSleeping, kPosition_5790);
				break;
			}

			getEntities()->drawSequenceLeft(kEntityFrancois, "605He");
			break;

		case 8:
			setCallback(9);
			setup_updateFromTime(450);
			break;

		case 9:
			getEntities()->exitCompartment(kEntityFrancois, kObjectCompartmentE, true);

			setCallback(10);
			setup_updateEntity(kCarGreenSleeping, kPosition_5790);
			break;

		case 10:
			setCallback(11);
			setup_function10();
			break;

		case 11:
			CALLBACK_ACTION();
			break;
		}
		break;

	case kAction190219584:
		setCallback(6);
		setup_enterExitCompartment("605Ef", kObjectCompartment6);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION_IIS(14, Francois, function14, ObjectIndex, EntityPosition)
	error("Francois: callback function 14 not implemented!");
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(15, Francois, function15)
	switch (savepoint.action) {
	default:
		break;

	case kActionDefault:
		setCallback(1);
		setup_function9();
		break;

	case kActionCallback:
		switch (getCallback()) {
		default:
			break;

		case 1:
			if (getData()->entityPosition >= getEntityData(kEntityPlayer)->entityPosition) {
				setCallback(3);
				setup_updateEntity(kCarRedSleeping, kPosition_540);
			} else {
				setCallback(2);
				setup_updateEntity(kCarRedSleeping, kPosition_9460);
			}
			break;

		case 2:
		case 3:
			setCallback(4);
			setup_updateFromTime(450);
			break;

		case 4:
			setCallback(5);
			setup_updateEntity(kCarRedSleeping, kPosition_5790);
			break;

		case 5:
			setCallback(6);
			setup_function10();
			break;

		case 6:
			setCallback(7);
			setup_updateFromTime(900);
			break;

		case 7:
			if (!getEntities()->isInsideCompartment(kEntityMmeBoutarel, kCarRedSleeping, kPosition_5790)) {
				CALLBACK_ACTION();
				break;
			}

			setCallback(8);
			setup_playSound("Fra2012");
			break;

		case 8:
			CALLBACK_ACTION();
			break;
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(16, Francois, function16)
	switch (savepoint.action) {
	default:
		break;

	case kActionNone:
		getData()->entityPosition = getEntityData(kEntityBoutarel)->entityPosition;
		getData()->location = getEntityData(kEntityBoutarel)->location;
		break;

	case kActionDefault:
		getObjects()->update(kObjectCompartmentD, kEntityPlayer, kObjectLocationNone, kCursorNormal, kCursorNormal);

		setCallback(1);
		setup_enterExitCompartment("605Cd", kObjectCompartmentD);
		break;

	case kActionCallback:
		switch (getCallback()) {
		default:
			break;

		case 1:
			getData()->location = kLocationOutsideCompartment;
			getData()->entityPosition = kPosition_5890;

			getSavePoints()->push(kEntityFrancois, kEntityMmeBoutarel, kAction101107728);

			setCallback(2);
			setup_updateEntity(kCarRestaurant, kPosition_850);
			break;

		case 2:
			getEntities()->clearSequences(kEntityFrancois);
			getSavePoints()->push(kEntityFrancois, kEntityBoutarel, kAction237889408);
			break;

		case 3:
			setCallback(4);
			setup_enterExitCompartment("605Id", kObjectCompartmentD);
			break;

		case 4:
			getObjects()->update(kObjectCompartmentD, kEntityPlayer, kObjectLocation2, kCursorKeepValue, kCursorKeepValue);
			getSavePoints()->push(kEntityFrancois, kEntityMmeBoutarel, kAction100957716);

			getData()->entityPosition = kPosition_5790;
			getData()->location = kLocationInsideCompartment;

			CALLBACK_ACTION();
			break;
		}
		break;

	case kAction100901266:
		setCallback(3);
		setup_updateEntity(kCarRedSleeping, kPosition_5790);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(17, Francois, chapter1)
	switch (savepoint.action) {
	default:
		break;

	case kActionNone:
		TIME_CHECK_CHAPTER1(setup_chapter1Handler);
		break;

	case kActionDefault:
		getData()->entityPosition = kPosition_5790;
		getData()->location = kLocationInsideCompartment;
		getData()->car = kCarRedSleeping;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(18, Francois, chapter1Handler)
	switch (savepoint.action) {
	default:
		break;

	case kActionNone:
		TIME_CHECK_CALLBACK_1(kTimeParisEpernay, params->param1, 1, setup_function11, kTime1093500);
		break;

	case kActionCallback:
		if (getCallback() == 1)
			setup_function19();
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(19, Francois, function19)
	switch (savepoint.action) {
	default:
		break;

	case kActionNone:
		TIME_CHECK_CALLBACK(kTime1161000, params->param1, 2, setup_function12);
		break;

	case kAction101107728:
		setCallback(1);
		setup_function16();
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(20, Francois, function20)
	if (savepoint.action == kActionDefault) {
		getData()->entityPosition = kPosition_5790;
		getData()->location = kLocationInsideCompartment;
		getData()->car = kCarRedSleeping;

		getEntities()->clearSequences(kEntityFrancois);
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(21, Francois, chapter2)
	switch (savepoint.action) {
	default:
		break;

	case kActionNone:
		setup_chapter2Handler();
		break;

	case kActionDefault:
		getEntities()->clearSequences(kEntityFrancois);

		getData()->entityPosition = kPosition_4689;
		getData()->location = kLocationInsideCompartment;
		getData()->car = kCarRestaurant;
		getData()->clothes = kClothesDefault;
		getData()->inventoryItem = kItemNone;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(22, Francois, chapter2Handler)
	switch (savepoint.action) {
	default:
		break;

	case kActionCallback:
		switch (getCallback()) {
		default:
			break;

		case 1:
			setCallback(2);
			setup_enterExitCompartment("605Id", kObjectCompartmentD);
			break;

		case 2:
			getObjects()->update(kObjectCompartmentD, kEntityPlayer, kObjectLocation2, kCursorKeepValue, kCursorKeepValue);
			getSavePoints()->push(kEntityFrancois, kEntityMmeBoutarel, kAction100957716);
			getData()->entityPosition = kPosition_5790;
			getData()->location = kLocationInsideCompartment;
			getEntities()->clearSequences(kEntityFrancois);
			setup_function23();
			break;
		}
		break;

	case kAction100901266:
		setCallback(1);
		setup_updateEntity(kCarRedSleeping, kPosition_5790);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(23, Francois, function23)
	error("Francois: callback function 23 not implemented!");
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(24, Francois, chapter3)
	switch (savepoint.action) {
	default:
		break;

	case kActionNone:
		setup_chapter3Handler();
		break;

	case kActionDefault:
		getEntities()->clearSequences(kEntityFrancois);

		getData()->entityPosition = kPosition_5790;
		getData()->location = kLocationInsideCompartment;
		getData()->car = kCarRedSleeping;
		getData()->clothes = kClothesDefault;
		getData()->inventoryItem = kItemNone;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(25, Francois, chapter3Handler)
	switch (savepoint.action) {
	default:
		break;

	case kActionNone:
		if (getEvent(kEventFrancoisShowBeetle) || getEvent(kEventFrancoisShowBeetleD))
			if (!getEvent(kEventFrancoisTradeWhistle) && !getEvent(kEventFrancoisTradeWhistleD))
				ENTITY_PARAM(0, 1) = 1;

		if (params->param2 && getEntities()->isInsideCompartment(kEntityMmeBoutarel, kCarRedSleeping, kPosition_5790) && !params->param1) {

			if (ENTITY_PARAM(0, 1) && getEntities()->isPlayerInCar(kCarRedSleeping)) {
				setCallback(2);
				setup_function15();
				break;
			}

label_callback_2:
			TIME_CHECK_CALLBACK(kTime2025000, params->param3, 3, setup_function12);

label_callback_3:
			TIME_CHECK_CALLBACK(kTime2052000, params->param4, 4, setup_function12);

label_callback_4:
			TIME_CHECK_CALLBACK(kTime2079000, params->param5, 5, setup_function12);

label_callback_5:
			TIME_CHECK_CALLBACK(kTime2092500, params->param6, 6, setup_function12);

label_callback_6:
			TIME_CHECK_CALLBACK(kTime2173500, params->param7, 7, setup_function12);

label_callback_7:
			TIME_CHECK_CALLBACK(kTime2182500, params->param8, 8, setup_function12);

label_callback_8:
			TIME_CHECK_CALLBACK(kTime2241000, CURRENT_PARAMS(1, 1), 9, setup_function12);

label_callback_9:
			if (!getInventory()->hasItem(kItemWhistle) && getInventory()->get(kItemWhistle)->location != kObjectLocation3) {
				TIME_CHECK_CALLBACK_1(kTime2011500, CURRENT_PARAMS(1, 2), 10, setup_function11, kTime2016000);

label_callback_10:
				TIME_CHECK_CALLBACK_1(kTime2115000, CURRENT_PARAMS(1, 3), 11, setup_function11, kTime2119500);
			}

label_callback_11:
			if (getInventory()->get(kItemWhistle)->location == kObjectLocation3) {
				if (getState()->time <= kTimeEnd)
					if (!getEntities()->isDistanceBetweenEntities(kEntityFrancois, kEntityPlayer, 2000) || !params->param4)
						params->param4 = getState()->time + 75;

				if (params->param4 < getState()->time || getState()->time > kTimeEnd) {
					params->param4 = kTimeInvalid;

					setCallback(12);
					setup_playSound("Fra2010");
					break;
				}

label_callback_12:
				TIME_CHECK_CALLBACK_3(kTime2040300, CURRENT_PARAMS(1, 5), 13, setup_function14, kObjectCompartmentE, kPosition_4840, "e");

label_callback_13:
				TIME_CHECK_CALLBACK_3(kTime2040300, CURRENT_PARAMS(1, 6), 14, setup_function14, kObjectCompartmentF, kPosition_4070, "f");

label_callback_14:
				TIME_CHECK_CALLBACK_3(kTime2040300, CURRENT_PARAMS(1, 7), 15, setup_function14, kObjectCompartmentB, kPosition_7500, "b");
			}
		}
		break;

	case kActionCallback:
		switch (getCallback()) {
		default:
			break;

		case 1:
			params->param2 = 1;
			break;

		case 2:
			goto label_callback_2;

		case 3:
			goto label_callback_3;

		case 4:
			goto label_callback_4;

		case 5:
			goto label_callback_5;

		case 6:
			goto label_callback_6;

		case 7:
			goto label_callback_7;

		case 8:
			goto label_callback_8;

		case 9:
			goto label_callback_9;

		case 10:
			goto label_callback_10;

		case 11:
			goto label_callback_11;

		case 12:
			getProgress().field_9C = 1;
			goto label_callback_12;

		case 13:
			goto label_callback_13;

		case 14:
			goto label_callback_14;
		}
		break;

	case kAction101107728:
		setCallback(1);
		setup_function16();
		break;

	case kAction189872836:
		params->param1 = 1;
		break;
	case kAction190390860:
		params->param1 = 0;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(26, Francois, chapter4)
	switch (savepoint.action) {
	default:
		break;

	case kActionNone:
		setup_chapter4Handler();
		break;

	case kActionDefault:
		getEntities()->clearSequences(kEntityFrancois);

		getData()->entityPosition = kPosition_5790;
		getData()->location = kLocationInsideCompartment;
		getData()->car = kCarRedSleeping;
		getData()->clothes = kClothesDefault;
		getData()->inventoryItem = kItemNone;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(27, Francois, chapter4Handler)
	if (savepoint.action == kAction101107728) {
		setCallback(1);
		setup_function16();
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(28, Francois, chapter5)
	switch (savepoint.action) {
	default:
		break;

	case kActionNone:
		setup_chapter5Handler();
		break;

	case kActionDefault:
		getEntities()->clearSequences(kEntityFrancois);

		getData()->entityPosition = kPosition_3969;
		getData()->location = kLocationInsideCompartment;
		getData()->car = kCarRestaurant;
		getData()->clothes = kClothesDefault;
		getData()->inventoryItem = kItemNone;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(29, Francois, chapter5Handler)
	if (savepoint.action == kActionProceedChapter5) {
		if (!getInventory()->hasItem(kItemWhistle)
		  && getInventory()->get(kItemWhistle)->location != kObjectLocation3)
		  getInventory()->setLocationAndProcess(kItemWhistle, kObjectLocation1);

		setup_function30();
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_FUNCTION(30, Francois, function30)
	switch (savepoint.action) {
	default:
		break;

	case kActionDefault:
		getData()->entityPosition = kPosition_5790;
		getData()->location = kLocationInsideCompartment;
		getData()->car = kCarRedSleeping;
		getData()->clothes = kClothesDefault;
		getData()->inventoryItem = kItemNone;
		break;

	case kAction135800432:
		setup_nullfunction();
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_NULL_FUNCTION(31, Francois)

} // End of namespace LastExpress