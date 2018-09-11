if(Preamble_count<12 && !preamble_flag)
                {
                    if(duration>260&&duration<660)
                    {
                        Preamble_count++;
                        if(Preamble_count==12)
            {
              preamble_flag=1;
              HCS_bit_counter=0;
                        }
                    }
          else
                    {
            Preamble_count=0;
                    }
                } else
                    {
                        Preamble_count=0;
                        phi=0;
                    }
                    //проверка правильности импульса
                  if(front == 1)
                 {
                    if(((duration > 300) && (duration < 900)) || HCS_bit_counter == 0){}
                      else
                      {
                        // неправильная пауза между импульсами
                        preamble_flag = 0;
                        //return;
                      }
                  }


                    if(preamble_flag)
                    {

                        if(duration>260 && duration<1000)
                        {
                            RF_bufer[65-HCS_bit_counter] = duration>600 ? 0:1;
                            HCS_bit_counter++;
                            if(HCS_bit_counter==65)
                                {
                                    //очистка переменных
                                    read_ID=0;

                                    LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_8);
                                    preamble_flag=0;
                                    reciver_full = 1;
                                    HCS_bit_counter=0;
                                    LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_8);
                                }
                        }
                    }



          //////////////////////////////////////////////////////////////////
          /* if(preamble_flag == 1)
            {

                if((duration > 600) && (duration < 1800))
                {
                  RF_bufer[65 - HCS_bit_counter] = (duration > 600) ? 0 : 1; // импульс больше, чем половина от Те * 3 поймали 0, иначе 1
                  HCS_bit_counter++;
                }
                else
                {
                  // сбой приема
                  preamble_flag=0;
                  return;
                }
              }
            */

////////////////////////////////////////////



                    /* USER CODE END LL_EXTI_LINE_8 */
                }