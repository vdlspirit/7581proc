#ifndef	SPI2_H_
#define	SPI2_H_


#define MY_SPI_MODULE_CLK       166666666   /* SYSCLK7  = CPU_Clk/6 in HZ */
#define SPI_MAX_FREQ            25000000    /* SPI Max frequency in Hz */
#define SPI_MIN_FREQ            5000000    /* SPI Min frequency in Hz */

/* SPI error status */
#define SPI_STATUS        Uint32           /* SPI error status type */
#define SPI_EFAIL         (SPI_STATUS)-1   /* General failure code */
#define SPI_EOK           0                /* General success code */


SPI_STATUS spi_claim_oed(uint32_t  cs,uint32_t  freq, uint8_t  NBit);
SPI_STATUS spi_rwreg_oed(uint8_t reg,uint16_t* data_out,uint16_t* data_in,uint8_t RW);
void spi_release_oed(void);
void spi_init_cmos_oed(int cs);
void spi_init_xilinx_cmos_oed(uint16_t command,uint16_t   cs);

//flash
void spi_cmos_flash_read_command(uint8_t command,uint32_t* addr);
void spi_cmos_flash_read_data(uint8_t command,uint16_t* data_in);

void spi_cmos_erase_all_flash(uint32_t size, uint8_t cs);
void spi_cmos_write_flash(uint32_t size, uint8_t cs,uint8_t *buff, uint32_t addr);
void spi_cmos_read_flash(uint32_t size, uint8_t cs,uint16_t *buff, uint32_t addr);


#endif
