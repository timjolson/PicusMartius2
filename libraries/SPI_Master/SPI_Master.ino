/*
Nathan Freitag
Steffen Kist
Usman Beg

Program: SPI_Master
Purpose: This program configures the arduino's serial peripheral
          interface as the master, and uses the SPI to transmit
          a string (a character array) to the other arduino
          serially.
 */

void setup()
{
  /* Runs the SPI_MasterInit function and begins
      serial communication at 9600 bits per second. */
  SPI_MasterInit();
  Serial.begin(9600);
}

void loop()
{
  /* Creates a character array which will hold the string
      to be sent over the serial communication. */
  char myData[30] = "This is a test!";


  /* Loop to transmit the character array one character
      at a time. The loop sends the characters until
      it reaches the end of the string. Each execution
      of the loop calls the SPI_MasterTransmit function,
      passing the current character of the array. */
  for(int i = 0; i < strlen(myData); i++)
  {
    SPI_MasterTransmit(myData[i]);
  }
  
  
  /* Prints the character string that is currently being
      to the serial monitor for debugging purposes. */
  Serial.println(myData);


  // Delays by 300ms before sending the next string.
  delay(300);
}


/* SPI_MasterInit function: sets up the control registers
    of the SPI to set it up as the master. */
void SPI_MasterInit(void)
{
  // Set MOSI and SCK output, all others input
  /* Port B Data Direction Register: sets pins B3 and
      B5 to outputs, and leaves the rest of the port B
      pins as inputs. These are the MOSI and SCK pins. */
  DDRB = (1<<DDB3)|(1<<DDB5);
  
  
  /* SPI Control Register: enables the SPE, MSTR, and
      SPR0 bits.  The SPE bit enables the SPI, the MSTR
      bit assigns this SPI as the master, and the SPR
      bits being set to 01 (SPR1=0, SPR0=1) causes the
      frequency divider to be set to /16. */
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}


/* SPI_MasterTransmit function: transmits the data. */
void SPI_MasterTransmit(char cData)
{
  /* SPI Data Register: loads the character bits into
      the register to be transmitted. */
  SPDR = cData;
  
  
  /* SPI Status Register: Do nothing until the SPI
      interrupt flag is set.  The flag is set when
      the current data transfer is complete. */
  while(!(SPSR & (1<<SPIF)))
  ;
}
