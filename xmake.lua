add_rules("mode.debug", "mode.release")
set_languages("cxx17")

target("client")
    add_rules("qt.widgetapp")
    add_links("Qt6Network")
    
    add_files("src/*.ui")
    add_files("src/*.cc")
    add_files("src/*.h")

    add_defines("QT_DEBUG")