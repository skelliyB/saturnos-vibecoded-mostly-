void kfgets(char* buffer, int max)
{
    int i = 0;

    while (1)
    {
        uint8_t sc = get_scancode();
        if (sc == 0) continue;

        char c = scancode_to_ascii(sc);
        if (c == 0) continue;

        // ENTER
        if (c == '\n')
        {
            buffer[i] = '\0';
            kprint("\n");
            return;
        }

        // BACKSPACE
        if (c == '\b')
        {
            if (i > 0) i--;
            continue;
        }

        // normal char
        if (i < max - 1)
        {
            buffer[i++] = c;

            char out[2] = {c, 0};
            kprint(out);
        }
    }
}