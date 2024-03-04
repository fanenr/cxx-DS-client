add_rules("mode.debug", "mode.release")

target("Demo")
    add_rules("qt.widgetapp")

    add_files("src/*.ui")
    add_files("src/*.cc")
    add_files("src/*.h")
