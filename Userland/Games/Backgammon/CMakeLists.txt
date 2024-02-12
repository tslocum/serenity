serenity_component(
    Backgammon
    RECOMMENDED
    TARGETS Backgammon
)

set(SOURCES
    main.cpp
    BackgammonWidget.cpp
)

serenity_app(Backgammon ICON app-chess)
target_link_libraries(Backgammon PRIVATE LibConfig LibFileSystemAccessClient LibGfx LibGUI LibCore LibMain LibDesktop)
