run:
	g++ -o word_shower --std=c++17 main.cpp falling_word.cpp word_shower.cpp ui.cpp state.cpp screen_menu.cpp screen_game.cpp screen_confirm.cpp drawing.cpp lab_color.cpp ../tui_engine/tui.cpp
	./word_shower
