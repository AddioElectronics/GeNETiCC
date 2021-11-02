#include "../geneticc_dma.h"

#if GENETICC_DMA_SYSTEM == GENETICC_DMA_SYSTEM_ATMELSTART_ARM


#include <compiler.h>
#include <err_codes.h>


/*
	Relying on Atmel Start to configure everything else.
*/
void geneticc_dma_system_init()
{
	/*	Setup in atmel start!
	*
	*	-Enable channel
	*	-Enable src and dst increment.
	*	-Set beat size to 4 bytes
	*	-Set priority level, and enable it
	*/
	
	//_dma_srcinc_enable(GENETICC_DMA_CHANNEL, true);
	//_dma_dstinc_enable(GENETICC_DMA_CHANNEL, true);
	
	
	
	//_dma_set_irq_state(GENETICC_DMA_CHANNEL, DMA_TRANSFER_COMPLETE_CB, true);
	
	//geneticc_dma_enabled = true;
}

bool geneticc_dma_wait()
{
	static uint32_t timeout = 0;
	
	//Wait for DMA to start if it hasn't.
	while ((DMAC->BUSYCH.reg & (1 << GENETICC_DMA_CHANNEL)) == 0 && (DMAC->PENDCH.reg & (1 << GENETICC_DMA_CHANNEL)) != 0)
	{
		if(++timeout >= GENETICC_DMA_TIMEOUT)
		{
			return false;
		}
	}
	
	//Wait for transfer to finish.
	while ((DMAC->BUSYCH.reg & (1 << GENETICC_DMA_CHANNEL)) != 0);

	return true;
}

bool __attribute__((__always_inline__)) geneticc_dma_busy()
{
	return (DMAC->BUSYCH.reg & (1 << GENETICC_DMA_CHANNEL)) != 0;
}

bool __attribute__((__always_inline__)) geneticc_dma_busy_timeout()
{
	static uint32_t timeout = 0;
	
	while( (DMAC->BUSYCH.reg & (1 << GENETICC_DMA_CHANNEL)) != 0)
	{
		if(++timeout >= GENETICC_DMA_TIMEOUT)
		{
			return false;
		}
	}
	
	return true;
}



int32_t _dma_disable_transaction(const uint8_t channel)
{
	hri_dmac_write_CHID_reg(DMAC, channel);
	hri_dmacdescriptor_clear_BTCTRL_VALID_bit(&_descriptor_section[channel]);
	hri_dmac_clear_CHCTRLA_ENABLE_bit(DMAC);
	hri_dmac_clear_SWTRIGCTRL_reg(DMAC, 1 << channel);

	return ERR_NONE;
}

void internal_geneticc_dma_register_system_callbacks()
{
	struct _dma_resource *res;
	_dma_get_channel_resource(&res, GENETICC_DMA_CHANNEL);
	
	res->dma_cb.transfer_done	= GENETICC_DMA_CALLBACK_TRANSFERDONE_PROTOTYPE_NAME;
	res->dma_cb.error			= GENETICC_DMA_CALLBACK_ERROR_PROTOTYPE_NAME;
}

//#elif GENETICC_DMA_SYSTEM == 

#endif