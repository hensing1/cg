# Infos zur allgemeinen Projektstruktur

Das Rendering der einzelnen Szenen passiert in `src/scenes.cpp`. Die darin definierten Methoden `render_scene_01`, `render_scene_02`, etc. werden im Rendering-Loop in `mainapp.cpp` aufgerufen.

Das Rendering jeder Szene wird über den gobalen `FRAME`-counter gesteuert. *Damit Szenen unabhängig voneinander entwickelt werden können, wird dieser Counter am Ende jeder Szene zurückgesetzt.*
