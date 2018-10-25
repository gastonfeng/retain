#include <Arduino.h>
#include <EEPROM.h>
EEPROMClass EEPROM;
class retain_EEPROM
{
  public:
    retain_EEPROM(uint16 *dat, uint16 dlen)
    {
        this->len = dlen / 2 ;
        buffer = (uint16 *)malloc(dlen + 4);
        buf = dat;
        for (int i = 0; i < this->len + 2; i++)
            buffer[i] = EEPROM.read(i);
        if (buffer[0] == 0x55aa && buffer[this->len + 1] == 0xaa55)
        {
            memcpy(dat, buffer + 1, this->len*2);
            status = load_ok;
        }
        else
        {
            EEPROM.format();
            status = load_failed;
        }
    }
    void loop()
    {
        // if (memcmp(buffer + 1, buf, this->len) != 0)
        {
            memcpy(buffer + 1, buf, len*2);
            buffer[0] = 0x55aa;
            buffer[this->len + 1] = 0xaa55;
            for (int i = 0; i < len + 2; i++)
                EEPROM.update(i, buffer[i]);
        }
    }
    enum
    {
        load_ok,
        load_failed
    };
    int status;

  private:
    uint16 len;
    uint16 *buffer, *buf;
};