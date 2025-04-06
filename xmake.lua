includes("@builtin/xpack")

add_requires("qt6core", "qt6widgets", "nlohmann_json", "cpp-httplib", "openssl3", { config = {ssl = true}})

target("spoverlay-app")
    add_rules("qt.widgetapp", "qt.moc")
    set_languages("cxx17")
    add_files("src/*.cpp", "ressources.qrc")
    add_files("src/*.h")
    add_installfiles("images/*.png")
    add_installfiles("README.md")
    add_packages("qt6core", "qt6widgets", "nlohmann_json", "openssl3", "cpp-httplib", {configs = {ssl = true}})
