run:
	g++ -o word_shower --std=c++17 main.cpp game_word.cpp game.cpp ui.cpp state.cpp screen_menu.cpp screen_game.cpp screen_confirm.cpp screen_scores.cpp screen_new_high_score.cpp drawing.cpp lab_color.cpp scores.cpp particle_emitter.cpp ../tui_engine/tui.cpp
	./word_shower
