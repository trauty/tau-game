set_project("tau-game")
set_version("0.0.1")

add_rules("plugin.compile_commands.autoupdate", {outputdir = "."})

-- the engine that configured this project, else the checkout beside it
option("tau_engine_dir")
    set_showmenu(true)
    set_description("The tau engine this project builds against")
option_end()

includes(path.join(get_config("tau_engine_dir") or path.join(os.scriptdir(), "..", "tau-engine"), "xmake", "tau.lua"))

target("tau-game")
    add_rules("tau.game", "tau.hotreload")

    add_files("src/**.cpp")
    add_includedirs("src")
target_end()
