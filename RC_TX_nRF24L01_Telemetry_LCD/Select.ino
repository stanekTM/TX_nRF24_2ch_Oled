//************************************************************************************************************************************************************************
// Select screen, step control for channels/values **********************************************************************************************************
//************************************************************************************************************************************************************************
void select()
{
  //------------------------------------------------------------------------
  // Start of screen display management
  //------------------------------------------------------------------------ 
  if (screen == 0) {             // Print MAIN screen
//    main_screen();
    draw_main_screen();
//    read_pots();                 // Macro again for stable ppm pulse
  }
  
  else {
    if (screen == 1) {           // Print MENU screen
//      menu_screen();
      draw_menu_screen();
    }
    
    if (screen == 2) {
      if (menuActual == 1) {     // Execute SERVO DIRECTION task
//        servo_dir_screen();
        draw_servo_dir_screen();
      }
      
      if (menuActual == 2) {     // Execute EPA task
//        epa_screen();
        draw_epa_screen();
      }
      
      if (menuActual == 3) {     // Execute MODEL SELECTION task
//        model_sel_screen();
        draw_model_sel_screen();
      }
      
      if (menuActual == 4) {     // Execute SAVE MODEL DATA task
        save_model_screen();
      }
      
      if (menuActual == 5) {     // Execute SUB TRIM task
//        sub_trim_screen();
        draw_sub_trim_screen(); 
      }
      
      if (menuActual == 6) {     // Execute MODEL NAME task
//        model_name_screen();
        draw_model_name_screen();
      }
      
      if (menuActual == 7) {     // Execute EXPO task
//        expo_screen();
        draw_expo_screen();
      }
    }
  }

//************************************************************************************************************************************************************************
//************************************************************************************************************************************************************************
//************************************************************************************************************************************************************************
  unsigned char pressedKey = read_button();

  // If pressed buttonSelect/Enter
  if (pressedKey == 2) {
    if (screen < 2) {
      screen++;
      menuActual = menuSubActual;
      menuSubActual = 1;
      menuSubModel = modelActual;
      modelPage = (menuSubModel) / 10;
    }
    
    else {
      //-------------------------------------------------------------------------
      // Added if Menu/Select button is pressed without menu cursor (do nothing)
      //-------------------------------------------------------------------------
      if (menuActual == 0) {
        screen--;
        menuActual--;
      }
      //---------------------------------------------------------------------------------------
      // Options for each menu
      //---------------------------------------------------------------------------------------
      if (menuActual == 1) {

        // Selection value for SERVO DIRECTION
        bitWrite(servoReverse, menuSubActual - 1, !bitRead(servoReverse, menuSubActual - 1));
      }
      
      if (menuActual == 2) {

        // Selection value for EPA
        if (epaSelection != 0xFF) {
          epaSelection = 0xFF;
        }
        
        else {
          epaSelection = menuSubActual - 1;
        }
      }

      // MODEL SELECTION data
      if (menuActual == 3) {

        // Selection value for MODEL SELECTION
        modelActual = menuSubModel;

        // Save data in eeprom
        EEPROM.update(actualModelEepromAddr, modelActual);
        storedDataEeprom(modelActual);

        screen = 0;
        menuActual = 0;

        delay(10);
      }

      if (menuActual == 5) {

        // Selection value for SUB TRIM
        if (subTrimSelection != 0xFF) {
          subTrimSelection = 0xFF;
        }
        
        else {
          subTrimSelection = menuSubActual - 1;
        }
      }
      
      if (menuActual == 6) {

        // Selection value for Set MODEL NAME
        if (modelNameSelection != 0xFF) {
          modelNameSelection = 0xFF;
        }
        
        else {
          modelNameSelection = menuSubActual - 1;
        }
      }

      if (menuActual == 7) {

        // Selection value for EXPO
        if (expoSelection != 0xFF) {
          expoSelection = 0xFF;
        }
        
        else {
          expoSelection = menuSubActual - 1;
        }
      }
    }
  }
  
//************************************************************************************************************************************************************************
// MIN step control for selecting channels/values (pressedKey == 3) ******************************************************************************************************
//************************************************************************************************************************************************************************
  if (pressedKey == 3) {

    switch (menuActual) {

  //---------------------------------------------------------------------------------
  // "EPA" screen step control for selecting channels/values (buttonDown)
  //---------------------------------------------------------------------------------
      case 2:
  
        // Initial value for EPA selection value
        if (epaSelection == 0xFF) {

          // Only first 3 values
          if (menuSubActual < 3) {
            menuSubActual++;
            
            if (screen == 0) {
              screen++;
            }
          }
        }
        
        // EPA step control for changing selected channel value(DOWN)
        else {
          if ((epaSelection % 1) == 0) { //% 2

            // Define Min for EPA value
            if (epa[epaSelection / 1] > 0) { //2

              // Define step value
              epa[epaSelection / 1]--; //2
            }
          }
        }
        break;
//***************************************************************************************
      //---------------------------------------------------------------------------------
      // "MODEL SELECTION" screen
      //---------------------------------------------------------------------------------
      case 3:
        //-----------------------------------------------
        // Added for Model Menu management
        //-----------------------------------------------
        // 0 to 4 items
        if (menuSubModel < MODELS - 1) {
          menuSubModel++;
          modelPage = (menuSubModel) / 10;
          
          if (screen == 0) screen++;
        }
        break;
//***************************************************************************************
      //---------------------------------------------------------------------------------
      // "SUB TRIM" screen step control for selecting channels/values (buttonDown)
      //---------------------------------------------------------------------------------
      case 5:

        // Initial value for SUB TRIM selection value
        if (subTrimSelection == 0xFF) {

          // Only first 2 channels
          if (menuSubActual < 2) {
            menuSubActual++;

            if (screen == 0) {
              screen++;
            }
          }
        }

        // SUB TRIM step control for changing selected channel value DOWN
        else {
          if ((subTrimSelection % 1)  == 0) {

            // Define Min SUB TRIM value
            if (subTrim[subTrimSelection / 1] > potCenter * -1) {

              // Define step value
              subTrim[subTrimSelection / 1] -= 1;
            }
          }
        }
        break;
//***************************************************************************************
      //---------------------------------------------------------------------------------
      // "MODEL NAME" screen step control for selecting values (buttonDown)
      //---------------------------------------------------------------------------------
      case 6:

        // Initial value for MODEL NAME selection value
        if (modelNameSelection == 0xFF) {

          // Only 5 characters
          if (menuSubActual < 5) { 
            menuSubActual++;
            
            if (screen == 0) {
              screen++;
            }
          }
        }
        
        // changing selected position character ASCII value(DOWN)
        else {
          if (modelName[modelNameSelection] > 0x20) {
          modelName[modelNameSelection]--;
          }
        }
        break;
//***************************************************************************************
      //---------------------------------------------------------------------------------
      // "EXPO" screen step control for selecting channels/values (buttonDown)
      //---------------------------------------------------------------------------------
      case 7:

        // Initial value for EXPO selection value
        if (expoSelection == 0xFF) {

          // Only first 2 channels
          if (menuSubActual < 2) {
            menuSubActual++;
            
            if (screen == 0) {
              screen++;
            }
          }
        }
        
        // EXPO step control for changing selected channel value(DOWN)
        else {
          if (expo[expoSelection] > 0) {

            // Define step value
            expo[expoSelection]--;
          }
        }
        break;
//***************************************************************************************

      default:

        if (menuSubActual < MENU_COUNT) { // 1 to 5 items
          menuSubActual++;
        }
        
        menuPage = (menuSubActual - 1) / 5;
        
        break;
    }
  }
  
//************************************************************************************************************************************************************************
// MAX step control for selecting channels/values (pressedKey == 1) ******************************************************************************************************
//************************************************************************************************************************************************************************
  if (pressedKey == 1) {
    
    switch (menuActual) {

      //---------------------------------------------------------------------------------
      // "EPA" screen step control for selecting channels/values (buttonUp)
      //---------------------------------------------------------------------------------
      case 2:

        // Initial value for EPA selection value
        if (epaSelection == 0xFF) {

          // Only first 2 channels
          if (menuSubActual < 2) {
            screen--;
            menuSubActual = 2;
            menuActual = 0;
          }
          
          else {
            menuSubActual--;
          }
        }
        
        // Step control for changing selected channel value UP
        else {
          if ((epaSelection % 1) == 0) {

            // Define Max for EPA value
            if (epa[epaSelection / 1] < epaMax) {

              // Define step value
              epa[epaSelection / 1]++;
            }
          }
        }
        break;
//***************************************************************************************
      //---------------------------------------------------------------------------------
      // "MODEL SELECTION" screen
      //---------------------------------------------------------------------------------
      case 3:
        //----------------------------------
        // Added for Model Menu management
        //----------------------------------
        if (menuSubModel > 0) {
          menuSubModel--;
          modelPage = (menuSubModel) / 10;
        }
        
        else {
          if (screen > 0) {
            screen--;
            menuSubModel = 1;
            menuSubActual = 3;
            menuActual = 0;
          }
        }
        break;
//***************************************************************************************
      //---------------------------------------------------------------------------------
      // "SUB TRIM" screen step control for selecting channels/values (buttonUp)
      //---------------------------------------------------------------------------------
      case 5:
        if (subTrimSelection == 0xFF) {

          // Only first 2 channels
          if (menuSubActual < 2) {
            screen--;
            menuSubActual = 5;
            menuActual = 0;
          }
          
          else {
            menuSubActual--;
          }
        }

        // SUB TRIM step control for changing selected channel UP
        else {
          if ((subTrimSelection % 1) == 0 ) {

            // Define max value for SUB TRIM
            if ((subTrim[subTrimSelection / 1]) < potCenter) {

              // Define step value
              subTrim[subTrimSelection / 1] += 1;
            }
          }
        }
        break;
//***************************************************************************************
      //---------------------------------------------------------------------------------
      // "MODEL NAME" screen step control for selecting values (buttonUp)
      //---------------------------------------------------------------------------------
      case 6:
        // Initial value for MODEL NAME selection value
        if (modelNameSelection == 0xFF) {

          // Only 2 characters
          if (menuSubActual < 2) {
            screen--;
            menuSubActual = 6;
            menuActual = 0;
          }
          
          else {
            menuSubActual--;
          }
        }
        
        // changing selected position character ASCII value(UP)
        else {
          if (modelName[modelNameSelection] < 0x7e)
            modelName[modelNameSelection]++;
        }
        break;
//***************************************************************************************
      //---------------------------------------------------------------------------------
      // "EXPO" screen step control for selecting channels/values (buttonUp)
      //---------------------------------------------------------------------------------
      case 7:

        // Initial value for EXPO selection value
        if (expoSelection == 0xFF) {
          
          // Only first 2 channels
          if (menuSubActual < 2) { 
            screen--;
            menuSubActual = 7;
            menuActual = 0;
          }
          
          else {
            menuSubActual--;
          }
        }
        
        // Step control for changing selected channel value UP
        else {
          // Define Max for EXPO value
          if (expo[expoSelection] < 9) {

            // Define step value
            expo[expoSelection]++;
          }
        }
        break;
//***************************************************************************************

      default:

        if (menuSubActual > 1) {
          menuSubActual--;
          menuSubActual = menuSubActual < 1 ? 1 : menuSubActual;
          menuPage = (menuSubActual - 1) / 5;
        }
        
        else {
          if (screen > 0) {
            screen--;
            menuSubActual = 1;
            menuActual = 0;
          }
        }
        break;
    }
  }

  if (pressedKey != 0) {
//    delay(KEY_DELAY);
  }
}
 
