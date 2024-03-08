add_rules("mode.debug", "mode.release")
set_languages("cxx17")

target("client")
    add_rules("qt.widgetapp")
    add_links("Qt6Network")
    
    add_includedirs("include")
    add_files("**.ui")
    add_files("**.cc")
    add_files("**.h")
