State control_sequence(char c) {
  if (debug_parsing) { Serial.print("CSI.start\n"); Serial.flush(); }
  control_sequence_param_pos = 0;
  param_temp_buffer_pos = 0;
  return control_sequence_entry(c);
}

State control_sequence_entry(char c) {
  switch (c)
  {
    case '-': case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': 
      if (debug_parsing) { Serial.print("CSI.param");  Serial.print((int)c); Serial.print("\n"); Serial.flush(); }
      param_temp_buffer_eat(c);
      return (State) &control_sequence_entry;

    case ';':
      if (debug_parsing) { Serial.print("CSI.newparam"); Serial.print((int)c); Serial.print("\n"); Serial.flush(); }
      param_temp_buffer_digest(); // Prepares for next parameter... or control code
      return (State) &control_sequence_entry;


    // These appear in order or final character, as they do in https://www.xfree86.org/current/ctlseqs.html
    // Unimplemented CSI codes from that document have a MISSING comment here

    case '@':
      param_temp_buffer_digest(1);

      for (int i = 0; i < control_sequence_param[0]; i++) {
        terminal_put(' ');
      }
      return (State) &initial_state;

    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
      param_temp_buffer_digest();

      // Absolute cursor pos
      if (c == 'H')
      {
        if (debug_parsing) { Serial.print("CSI.H\n"); Serial.flush(); }
        current_cursor.y =  control_sequence_param[0];
        current_cursor.x = control_sequence_param_pos >= 1 ? control_sequence_param[1] : 1;
      }
      else
      {
        // Line based (E,F)
        if (c == 'E' || c == 'F')
        {
          if (debug_parsing) { Serial.print("CSI.EF"); Serial.print((int)c); Serial.print("\n"); Serial.flush(); }
          current_cursor.x = 1;
          current_cursor.y += (c == 'E' ? 1 : -1) * control_sequence_param[0];
        }
        else
        {
        if (debug_parsing) { Serial.print("CSI.relative"); Serial.print((int)c); Serial.print("\n"); Serial.flush(); }
          current_cursor.x = (c == 'G') ? control_sequence_param[0] : current_cursor.x;

          // Relative
          current_cursor.x += ((c == 'C' || c == 'D') ? ((c == 'D' ? -1 : 1) * control_sequence_param[0]) : 0);
          current_cursor.y += ((c == 'A' || c == 'B') ? ((c == 'A' ? -1 : 1) * control_sequence_param[0]) : 0);
        }
      }
      return (State) &initial_state;

    case 'I':
      param_temp_buffer_digest(1);
      for (int i = 0; i < control_sequence_param[0]; i++) {
        current_cursor.x = ((current_cursor.x / tab_size) + 1) * tab_size;
      }
      return (State) &initial_state;
    
    case 'J': // Clear screen
      if (debug_parsing) Serial.print("CSI.J\n");
      param_temp_buffer_digest(0);
      terminal_clear(control_sequence_param[0]);
      return (State) &initial_state;

    // MISSING CSI ? Ps J

    case 'K':

      param_temp_buffer_digest(0);
      terminal_clear_line(current_cursor.x, current_cursor.y, control_sequence_param[0]);
      return (State) &initial_state;

    // MISSING: CSI ? Ps K

    case 'L':
      param_temp_buffer_digest(1);
      for (int i = 0; i < control_sequence_param[0]; i++) {
        terminal_scroll(current_cursor.y, terminal_height, 0);
        terminal_clear_line(0, current_cursor.y, 2);
      }
      return (State) &initial_state;
    
    case 'M':
      param_temp_buffer_digest(1);
      for (int i = 0; i < control_sequence_param[0]; i++) {
        terminal_scroll(current_cursor.y, terminal_height, 1);
        terminal_clear_line(0, scroll_region.lower, 2);
      }
      return (State) &initial_state;

    case 'P':
      param_temp_buffer_digest(1);
      for (int i = 0; i < control_sequence_param[0]; i++) {
        terminal_scroll_line(current_cursor.y, current_cursor.x, terminal_width, 1);
        TERM(terminal_width, current_cursor.y) = ' ';
      }
      return (State) &initial_state;

    case 'S':
      param_temp_buffer_digest(1);
      for (int i = 0; i < control_sequence_param[0]; i++) {
        terminal_scroll(0, terminal_height, 1);
        terminal_clear_line(0, scroll_region.lower, 2);
      }
      return (State) &initial_state;

    case 'T':
      param_temp_buffer_digest(1);
      for (int i = 0; i < control_sequence_param[0]; i++) {
        terminal_scroll(0, terminal_height, 0);
        terminal_clear_line(0, scroll_region.upper, 2);
      }
      return (State) &initial_state;

    case 'X':
      param_temp_buffer_digest(1);
      for (int i = 0; i < control_sequence_param[0]; i++) {
        TERM(current_cursor.x + i, current_cursor.y) = ' ';
      }
      return (State) &initial_state;

    case 'Z':
      param_temp_buffer_digest(1);
      for (int i = 0; i < control_sequence_param[0]; i++) {
        current_cursor.x = ((current_cursor.x / tab_size) - 1) * tab_size;
      }
      return (State) &initial_state;

    case '`':
      // MISSING: CSI Pm `
      return (State) &initial_state;

    case 'b':
      param_temp_buffer_digest(1);
      terminal_scroll_line(current_cursor.y, current_cursor.x-1, current_cursor.x+control_sequence_param[0]-1, 0);
      return (State) &initial_state;

    case 'c':
      // Tell the user we're a VT220 with a printer (our wifi?) and no extra features...
      Serial.print("\x1b[?60;2;c");
      Serial.flush();

      return (State) &initial_state;




   
    case 't': // Other control seqs https://www.xfree86.org/current/ctlseqs.html
      if (debug_parsing) Serial.print("CSI.P\n");
      param_temp_buffer_digest();
      switch (control_sequence_param[0])
      {
        case 1:
          switch (control_sequence_param[1])
          {
            case 8: // Report the size of the text area in characters as CSI 8 ; height ; width t
            case 9: // Report the size of the screen in characters as CSI 9 ; height ; width t
              Serial.print("[");
              Serial.print(terminal_height);
              Serial.print(";");
              Serial.print(terminal_width);
              Serial.print("t");
              Serial.flush();
          }
      }
      return (State) &initial_state;


    default:
      return (State) &initial_state;
  }
}
