// MIT License
//
// Copyright (c) 2021 Devon Powell
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <genesis.h>

#include "game.h"
#include "sprites.h"
#include "utilities.h"

#define LOGO_WIDTH 200
#define LOGO_HEIGHT 48
#define LOGO_FADE_TIME 300  // milliseconds
#define LOGO_START_POSITION_Y (-LOGO_HEIGHT)
#define LOGO_END_POSITION_Y 64

static bool g_run_menu_exit;

static void joyHandlerMenu(u16 _joy, u16 _changed, u16 _state) {
  if (_state & _changed & BUTTON_START) {
    g_run_menu_exit = TRUE;
  }
}

void processGameMenu() {
  JOY_setEventHandler(NULL);

  const u16 screenWidth = VDP_getScreenWidth();
  const s16 titlePositionX = (screenWidth - LOGO_WIDTH) / 2;
  Sprite* title =
      SPR_addSpriteSafe(&titleSprite, titlePositionX, LOGO_START_POSITION_Y,
                        TILE_ATTR(PAL1, 0, FALSE, FALSE));
  const u16 numFrames = timeToFrames(LOGO_FADE_TIME);

  PAL_fadeInPalette(PAL1, titleSprite.palette->data, numFrames, TRUE);

  const f16 distance = intToFix16(LOGO_END_POSITION_Y - LOGO_START_POSITION_Y);
  const f16 increment = fix16Div(distance, intToFix16(numFrames));
  f16 titlePositionY = intToFix16(LOGO_START_POSITION_Y);

  while (PAL_isDoingFade()) {
    titlePositionY = fix16Add(titlePositionY, increment);

    SPR_setPosition(title, titlePositionX, fix16ToInt(titlePositionY));
    SPR_update();
    SYS_doVBlankProcess();
  }

  SPR_setPosition(title, titlePositionX, LOGO_END_POSITION_Y);
  showText("PRESS START BUTTON", 16);
  JOY_setEventHandler(&joyHandlerMenu);

  g_run_menu_exit = FALSE;

  while (!g_run_menu_exit) {
    SPR_update();
    SYS_doVBlankProcess();
  }

  clearText(16);
  JOY_setEventHandler(NULL);
  PAL_fadeOutPalette(PAL1, 30, TRUE);

  while (PAL_isDoingFade()) {
    SPR_update();
    SYS_doVBlankProcess();
  }

  setGameState(STATE_STAGE);
  SPR_releaseSprite(title);
  SPR_update();
  SYS_doVBlankProcess();
}
