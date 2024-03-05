add_rules("mode.debug", "mode.release")
set_languages("cxx17")

target("Demo")
    add_rules("qt.widgetapp")

    add_files("src/*.ui")
    add_files("src/*.cc")
    add_files("src/*.h")
