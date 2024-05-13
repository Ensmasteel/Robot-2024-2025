string = """
    new StaticAction(SOLAR_LEFT_ON),
            new MoveAction(VectorOriented(0.70f, 1.77, 0.0f), false, false, true, true),
            new MoveAction(VectorOriented(0.70f, 1.77, -0.9*PI/4), true, false, true, true),
            new StaticAction(SOLAR_LEFT_OFF,true),
            new MoveAction(VectorOriented(1.0f, 1.4f, -PI/2), false, false, true, true),
            new MoveAction(VectorOriented(0.85f, 1.2f, -3*PI/4), false, false, true, true),
            new StaticAction(CLOSE_CLAWS),
            new MoveAction(VectorOriented(0.94f, 1.69f, -PI/2), false, true, true, true),
            new StaticAction(START_MAGNET,true),
            new MoveAction(VectorOriented(0.94f, 1.71f, -PI/2), false, true, true, true),
            new MoveAction(VectorOriented(0.94f, 1.35f, -PI/2), false, false, true, true),
            new MoveAction(VectorOriented(1.11f, 1.73f, -PI/2), false, true, true, true),
            new MoveAction(VectorOriented(1.11f, 1.77f, -PI/2), false, true, true, true, 1000),
            new MoveAction(VectorOriented(1.23f, 1.47f, -PI/4), false, false, true, true),
            new MoveAction(VectorOriented(0.26f, 1.72f, 0.0f), false, true, true, true),
            new StaticAction(SHUTDOWN_MAGNET,true),
            new StaticAction(RAISE_CLAWS,true),
            new MoveAction(VectorOriented(0.45f, 1.72f, 0.0f), false, false, true, true),
            new MoveAction(VectorOriented(0.45f, 1.72f, PI), true, false, true, true),
            new MoveAction(VectorOriented(0.22f, 1.72f, PI), false, false, true, true),
            new StaticAction(OPEN_CLAWS),
            new MoveAction(VectorOriented(0.42f, 1.72f, PI), false, true, true, true),
            new StaticAction(LOWER_CLAWS,true),
            new MoveAction(VectorOriented(0.42f, 1.72f, -3*PI/8), true, false, true, true),
            new MoveAction(VectorOriented(0.90f, 0.78f, -3*PI/8), false, false, true, true),
            new MoveAction(VectorOriented(1.12f, 0.70f, 0.0f), false, false, true, true),
            new StaticAction(CLOSE_CLAWS),
            //  La suite des valeurs en X est affectée de -4 cm
            new MoveAction(VectorOriented(0.24f, 0.68f, 0.0f), false, true, true, true),
            new StaticAction(START_MAGNET,true),
            new MoveAction(VectorOriented(0.21f, 0.68f, 0.0f), false, true, true, true),
            new MoveAction(VectorOriented(0.54f, 0.5f, 0.0f), false, false, true, true),
            new MoveAction(VectorOriented(0.16f, 0.5f, 0.0f), false, true, true, true),
            new MoveAction(VectorOriented(0.14f, 0.5f, 0.0f), false, true, true, true, 1000),
            new MoveAction(VectorOriented(0.21f, 0.5f, 0.0f), false, false, true, true),
            new MoveAction(VectorOriented(0.21f, 0.5f, PI/2), true, false, true, true),
            new MoveAction(VectorOriented(0.21f, 0.23f, PI/2), false, true, true, true),
            new StaticAction(SHUTDOWN_MAGNET,true),
            new StaticAction(RAISE_CLAWS,true),
            new MoveAction(VectorOriented(0.21f, 0.36f, PI/2), false, false, true, true),
            new MoveAction(VectorOriented(0.21f, 0.36f, -PI/2), true, false, true, true),
            new MoveAction(VectorOriented(0.21f, 0.22f, -PI/2), false, false, true, true),
            new StaticAction(OPEN_CLAWS),
            new MoveAction(VectorOriented(0.21f, 0.30f, -PI/2), false, true, true, true)
"""

splitted = string.split("VectorOriented(")
result = splitted[0]
for s in splitted[1:]:
    result += "VectorOriented("
    entries = s.split(")", 1)[0].split(",")
    new_entries = "3.0f - " + entries[0] + ", " + entries[1] + ", " + "normalizeAngle(PI - " + entries[2] + "))"
    result += new_entries
    result += ")".join(s.split(")")[1:])

result = "SOLAR_R_TMP".join(result.split("SOLAR_LEFT"))
result = "SOLAR_LEFT".join(result.split("SOLAR_RIGHT"))
result = "SOLAR_RIGHT".join(result.split("SOLAR_R_TMP"))

print(result)
