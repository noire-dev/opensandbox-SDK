echo off

echo ---------------------------------------
echo QVM Compiler
echo Compile: game.qvm, cgame.qvm, ui.qvm
echo ---------------------------------------

cd ..

mkdir windows
mkdir windows\build
mkdir windows\game
mkdir windows\game\qvm
mkdir windows\build\game

copy make_scripts\asm.exe  windows\build\game\
copy make_scripts\lcc.exe  windows\build\game\
copy make_scripts\cpp.exe  windows\build\game\
copy make_scripts\rcc.exe  windows\build\game\
copy make_scripts\game.q3asm windows\build\

set LIBRARY=
set INCLUDE=

set cc=lcc -DGAME -DQVM -S -I..\..\..\code\game -I..\..\..\code\qcommon %1

cd windows\build\game

:: ########################################
:: Files to compile to           qagame.qvm
:: ########################################

%cc% ../../../code/game/ai_main.c
%cc% ../../../code/game/bg_lib.c
%cc% ../../../code/game/bg_misc.c
%cc% ../../../code/game/bg_pmove.c
%cc% ../../../code/game/bg_slidemove.c
%cc% ../../../code/game/g_active.c
%cc% ../../../code/game/g_alloc.c
%cc% ../../../code/game/g_bot.c
%cc% ../../../code/game/g_client.c
%cc% ../../../code/game/g_cmds.c
%cc% ../../../code/game/g_combat.c
%cc% ../../../code/game/g_items.c
%cc% ../../../code/game/g_main.c
%cc% ../../../code/game/g_misc.c
%cc% ../../../code/game/g_mover.c
%cc% ../../../code/game/g_physics.c
%cc% ../../../code/game/g_sandbox.c
%cc% ../../../code/game/g_session.c
%cc% ../../../code/game/g_spawn.c
%cc% ../../../code/game/g_svcmds.c
%cc% ../../../code/game/g_target.c
%cc% ../../../code/game/g_team.c
%cc% ../../../code/game/g_trigger.c
%cc% ../../../code/game/g_utils.c
%cc% ../../../code/game/g_weapon.c

copy ..\..\..\code\game\g_syscalls.asm ..
copy ..\..\..\code\q_sharedsyscalls.asm ..

%cc% ../../../code/qcommon/q_cvars.c
%cc% ../../../code/qcommon/q_math.c
%cc% ../../../code/qcommon/q_shared.c
%cc% ../../../code/qcommon/js_main.c
%cc% ../../../code/qcommon/js_func.c

echo -----------------
echo game.qvm compiled
echo -----------------

asm -f ../game

:: ########################################
:: End of compilation files, add yours here
:: ########################################

cd ..\..\..
cd make_scripts
cd ..

mkdir windows
mkdir windows\build
mkdir windows\game
mkdir windows\game\qvm
mkdir windows\build\cgame

copy make_scripts\asm.exe  windows\build\cgame\
copy make_scripts\lcc.exe  windows\build\cgame\
copy make_scripts\cpp.exe  windows\build\cgame\
copy make_scripts\rcc.exe  windows\build\cgame\
copy make_scripts\cgame.q3asm windows\build\

set LIBRARY=
set INCLUDE=

set cc=lcc -DCGAME -DQVM -S -I..\..\..\code\cgame -I..\..\..\code\ui -I..\..\..\code\qcommon %1

cd windows\build\cgame

:: ########################################
:: Files to compile to            сgame.qvm
:: ########################################

%cc% ../../../code/cgame/cg_consolecmds.c
%cc% ../../../code/cgame/cg_draw.c
%cc% ../../../code/cgame/cg_drawtools.c
%cc% ../../../code/cgame/cg_effects.c
%cc% ../../../code/cgame/cg_ents.c
%cc% ../../../code/cgame/cg_event.c
%cc% ../../../code/cgame/cg_info.c
%cc% ../../../code/cgame/cg_localents.c
%cc% ../../../code/cgame/cg_main.c
%cc% ../../../code/cgame/cg_marks.c
%cc% ../../../code/cgame/cg_players.c
%cc% ../../../code/cgame/cg_playerstate.c
%cc% ../../../code/cgame/cg_predict.c
%cc% ../../../code/cgame/cg_scoreboard.c
%cc% ../../../code/cgame/cg_servercmds.c
%cc% ../../../code/cgame/cg_snapshot.c
%cc% ../../../code/cgame/cg_unlagged.c
%cc% ../../../code/cgame/cg_view.c
%cc% ../../../code/cgame/cg_weapons.c

%cc% ../../../code/game/bg_lib.c
%cc% ../../../code/game/bg_misc.c
%cc% ../../../code/game/bg_pmove.c
%cc% ../../../code/game/bg_slidemove.c

copy  ..\..\..\code\cgame\cg_syscalls.asm ..
copy ..\..\..\code\q_sharedsyscalls.asm ..
copy ..\..\..\code\q_sharedsyscalls_client.asm ..

%cc% ../../../code/qcommon/q_cvars.c
%cc% ../../../code/qcommon/q_math.c
%cc% ../../../code/qcommon/q_shared.c
%cc% ../../../code/qcommon/js_main.c
%cc% ../../../code/qcommon/js_func.c

echo -----------------
echo cgame.qvm compiled
echo -----------------

asm -f ../cgame

:: ########################################
:: End of compilation files, add yours here
:: ########################################

cd ..\..\..
cd make_scripts
cd ..

mkdir windows
mkdir windows\build
mkdir windows\game
mkdir windows\game\qvm
mkdir windows\build\ui

copy make_scripts\asm.exe  windows\build\ui\
copy make_scripts\lcc.exe  windows\build\ui\
copy make_scripts\cpp.exe  windows\build\ui\
copy make_scripts\rcc.exe  windows\build\ui\
copy make_scripts\ui.q3asm windows\build\

set LIBRARY=
set INCLUDE=

set cc=lcc -DUI -DQVM -S -I..\..\..\code\ui -I..\..\..\code\qcommon %1

cd windows\build\ui

:: ########################################
:: Files to compile to               ui.qvm
:: ########################################

%cc% ../../../code/ui/ui_atoms.c
%cc% ../../../code/ui/ui_connect.c
%cc% ../../../code/ui/ui_controls.c
%cc% ../../../code/ui/ui_gameinfo.c
%cc% ../../../code/ui/ui_main.c
%cc% ../../../code/ui/ui_menu.c
%cc% ../../../code/ui/ui_newgame.c
%cc% ../../../code/ui/ui_options.c
%cc% ../../../code/ui/ui_playermodel.c
%cc% ../../../code/ui/ui_players.c
%cc% ../../../code/ui/ui_qmenu.c
%cc% ../../../code/ui/ui_servers.c
%cc% ../../../code/ui/ui_settings.c
%cc% ../../../code/ui/ui_spawnmenu.c

copy  ..\..\..\code\ui\ui_syscalls.asm ..
copy ..\..\..\code\q_sharedsyscalls.asm ..
copy ..\..\..\code\q_sharedsyscalls_client.asm ..

%cc% ../../../code/game/bg_lib.c
%cc% ../../../code/game/bg_misc.c

%cc% ../../../code/qcommon/q_cvars.c
%cc% ../../../code/qcommon/q_math.c
%cc% ../../../code/qcommon/q_shared.c
%cc% ../../../code/qcommon/js_main.c
%cc% ../../../code/qcommon/js_func.c

echo -----------------
echo ui.qvm compiled
echo -----------------

asm -f ../ui

:: ########################################
:: End of compilation files, add yours here
:: ########################################

cd ..\..\..
cd make_scripts
cd ..\..

:: ##############################################################
:: Replace "default" here with the name of your mod
:: Replace "opensandbox-SDK" here with the name of your SDK folder
:: Replace "opensandbox" here with the name of your game folder
:: ##############################################################

copy opensandbox-SDK\windows\game\qvm\qagame.qvm opensandbox\game\core.default\qvm\qagame.qvm
copy opensandbox-SDK\windows\game\qvm\cgame.qvm opensandbox\game\core.default\qvm\cgame.qvm
copy opensandbox-SDK\windows\game\qvm\ui.qvm opensandbox\game\core.default\qvm\ui.qvm

color 0A
pause
