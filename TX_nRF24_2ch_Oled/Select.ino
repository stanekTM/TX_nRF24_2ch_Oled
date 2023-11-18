
//*********************************************************************************************************************
// Select screen, step control for channels/values
//*********************************************************************************************************************
void select()
{
  // Print MAIN screen
  if (screen == 0)
  {
    draw_main_screen();
  }
  else
  {
    // Print MENU screen
    if (screen == 1)
    {
      draw_menu_screen();
    }
    
    if (screen == 2)
    {
      // Execute REVERSE task
      if (menuActual == 1)
      {
        draw_reverse_screen();
      }
      
      // Execute EPA task
      if (menuActual == 2)
      {
        draw_epa_screen();
      }
      
      // Execute MODEL SELECT task
      if (menuActual == 3)
      {
        draw_model_select_screen();
      }
      
      // Execute SAVE MODEL task
      if (menuActual == 4)
      {
        save_model_screen();
      }
      
      // Execute SUB TRIM task
      if (menuActual == 5)
      {
        draw_sub_trim_screen();
      }
      
      // Execute MODEL NAME task
      if (menuActual == 6)
      {
        draw_model_name_screen();
      }
      
      // Execute EXPO task
      if (menuActual == 7)
      {
        draw_expo_screen();
      }
    }
  }
  
  
  unsigned char pressedKey = read_button();
  
  // If pressed button SELECT
  if (pressedKey == 2)
  {
    if (screen < 2)
    {
      screen++;
      menuActual = menuSubActual;
      menuSubActual = 1;
      menuSubModel = modelActual;
      modelPage = (menuSubModel) / 10;
    }
    else
    {
      // Added if Menu/button SELECT is pressed without menu cursor (do nothing)
      if (menuActual == 0)
      {
        screen--;
        menuActual--;
      }
      
      // Options for each menu
      if (menuActual == 1)
      {
        // Selection value for REVERSE
        bitWrite(reverse, menuSubActual - 1, !bitRead(reverse, menuSubActual - 1));
      }
      
      if (menuActual == 2)
      {
        // Selection value for EPA
        if (epaSelection != 0xFF)
        {
          epaSelection = 0xFF;
        }
        else
        {
          epaSelection = menuSubActual - 1;
        }
      }
      
      // MODEL SELECT data
      if (menuActual == 3)
      {
        // Selection value for MODEL SELECT
        modelActual = menuSubModel;
        
        // Save data in eeprom
        EEPROM.update(ACTUAL_MODEL_EEPROM_ADDR, modelActual);
        storedDataEeprom(modelActual);
        
        screen = 0;
        menuActual = 0;
        
        delay(10);
      }
      
      if (menuActual == 5)
      {
        // Selection value for SUB TRIM
        if (subTrimSelection != 0xFF)
        {
          subTrimSelection = 0xFF;
        }
        else
        {
          subTrimSelection = menuSubActual - 1;
        }
      }
      
      if (menuActual == 6)
      {
        // Selection value for Set MODEL NAME
        if (modelNameSelection != 0xFF)
        {
          modelNameSelection = 0xFF;
        }
        else
        {
          modelNameSelection = menuSubActual - 1;
        }
      }
      
      if (menuActual == 7)
      {
        // Selection value for EXPO
        if (expoSelection != 0xFF)
        {
          expoSelection = 0xFF;
        }
        else
        {
          expoSelection = menuSubActual - 1;
        }
      }
    }
  }
  
  // min step control for selecting channels/values (button DOWN)
  if (pressedKey == 3)
  {
    switch (menuActual)
    {
      // EPA
      case 2:
      // Initial value for EPA selection value
      if (epaSelection == 0xFF)
      {
        // Only first 4 values
        if (menuSubActual < 4)
        {
          menuSubActual++;
          
          if (screen == 0)
          {
            screen++;
          }
        }
      }
      else
      {
        // EPA step control for changing selected channel value(DOWN)
        if ((epaSelection % 1) == 0)
        {
          // Define min for EPA value
          if (epa[epaSelection / 1] > 0)
          {
            // Define step value
            epa[epaSelection / 1]--;
          }
        }
      }
      break;
      
      // MODEL SELECT
      case 3:
      
      if (menuSubModel < MODELS - 1)
      {
        menuSubModel++;
        modelPage = (menuSubModel) / 10;
        
        if (screen == 0)
        {
          screen++;
        }
      }
      break;
      
      // SUB TRIM
      case 5:
      
      // Initial value for SUB TRIM selection value
      if (subTrimSelection == 0xFF)
      {
        // Only first 2 channels
        if (menuSubActual < 2)
        {
          menuSubActual++;
          
          if (screen == 0)
          {
            screen++;
          }
        }
      }
      else
      {
        // SUB TRIM step control for changing selected channel value DOWN
        if ((subTrimSelection % 1)  == 0)
        {
          // Define Min SUB TRIM value
          if (subTrim[subTrimSelection / 1] > SUB_TRIM_MAX * -1)
          {
            // Define step value
            subTrim[subTrimSelection / 1] -= 1;
          }
        }
      }
      break;
      
      // MODEL NAME
      case 6:
      
      // Initial value for MODEL NAME selection value
      if (modelNameSelection == 0xFF)
      {
        // Only 5 characters
        if (menuSubActual < 5)
        {
          menuSubActual++;
          
          if (screen == 0)
          {
            screen++;
          }
        }
      }
      else
      {
        // changing selected position character ASCII value(DOWN)
        if (modelName[modelNameSelection] > 0x20)
        {
          modelName[modelNameSelection]--;
        }
      }
      break;
      
      // EXPO
      case 7:
      
      // Initial value for EXPO selection value
      if (expoSelection == 0xFF)
      {
        // Only first 2 channels
        if (menuSubActual < 2)
        {
          menuSubActual++;
          
          if (screen == 0)
          {
            screen++;
          }
        }
      }
      else
      {
        // EXPO step control for changing selected channel value(DOWN)
        if (expo[expoSelection] > 0)
        {
          // Define step value
          expo[expoSelection]--;
        }
      }
      break;
      
      // DEFAULT
      default:
      
      if (menuSubActual < MENU_COUNT) // 1 to 5 items
      {
        menuSubActual++;
      }
      
      menuPage = (menuSubActual - 1) / 5;
      break;
    }
  }
  
  //************************************************************
  // max step control for selecting channels/values (button UP)
  //************************************************************
  if (pressedKey == 1)
  {
    switch (menuActual)
    {
      // EPA
      case 2:
      
      // Initial value for EPA selection value
      if (epaSelection == 0xFF)
      {
        // Only first 2 channels
        if (menuSubActual < 2)
        {
          screen--;
          menuSubActual = 2;
          menuActual = 0;
        }
        else
        {
          menuSubActual--;
        }
      }
      else
      {
        // Step control for changing selected channel value UP
        if ((epaSelection % 1) == 0)
        {
          // Define max for EPA value
          if (epa[epaSelection / 1] < EPA_MAX)
          {
            // Define step value
            epa[epaSelection / 1]++;
          }
        }
      }
      break;
      
      // MODEL SELECT
      case 3:
      
      if (menuSubModel > 0)
      {
        menuSubModel--;
        modelPage = (menuSubModel) / 10;
      }
      else
      {
        if (screen > 0)
        {
          screen--;
          menuSubModel = 1;
          menuSubActual = 3;
          menuActual = 0;
        }
      }
      break;
      
      // SUB TRIM
      case 5:
      
      if (subTrimSelection == 0xFF)
      {
        // Only first 2 channels
        if (menuSubActual < 2)
        {
          screen--;
          menuSubActual = 5;
          menuActual = 0;
        }
        else
        {
          menuSubActual--;
        }
      }
      else
      {
        // SUB TRIM step control for changing selected channel UP
        if ((subTrimSelection % 1) == 0 )
        {
          // Define max value for SUB TRIM
          if ((subTrim[subTrimSelection / 1]) < SUB_TRIM_MAX)
          {
            // Define step value
            subTrim[subTrimSelection / 1] += 1;
          }
        }
      }
      break;
      
      // MODEL NAME
      case 6:
      
      // Initial value for MODEL NAME selection value
      if (modelNameSelection == 0xFF)
      {
        // Only 2 characters
        if (menuSubActual < 2)
        {
          screen--;
          menuSubActual = 6;
          menuActual = 0;
        }
        else
        {
          menuSubActual--;
        }
      }
      else
      {
        // changing selected position character ASCII value(UP)
        if (modelName[modelNameSelection] < 0x7e)
        {
          modelName[modelNameSelection]++;
        }
      }
      break;
      
      // EXPO
      case 7:
      
      // Initial value for EXPO selection value
      if (expoSelection == 0xFF)
      {
        // Only first 2 channels
        if (menuSubActual < 2)
        {
          screen--;
          menuSubActual = 7;
          menuActual = 0;
        }
        else
        {
          menuSubActual--;
        }
      }
      else
      {
        // Step control for changing selected channel value UP
        if (expo[expoSelection] < 9)
        {
          // Define step value
          expo[expoSelection]++;
        }
      }
      break;
      
      // DEFAULT
      default:
      
      if (menuSubActual > 1)
      {
        menuSubActual--;
        menuSubActual = menuSubActual < 1 ? 1 : menuSubActual;
        menuPage = (menuSubActual - 1) / 5;
      }
      else
      {
        if (screen > 0)
        {
          screen--;
          menuSubActual = 1;
          menuActual = 0;
        }
      }
      break;
      
    }
  }
}
 
