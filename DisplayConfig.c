

const BYTE 	DisplayConfAdd[48]=
{
	0x01, // Set SS and SM bit
	0x02, // Set line inversion
	0x03, // Set Write direction
	0x04, // Set Scaling function off
	0x08, // Set BP and FP
	0x09, // Set non-display area
	0x0A, // Frame marker control
	0x0C, // Set interface control
	0x0D, // Frame marker Position
	0x0F, // Set RGB interface 
	//--------------- Power On Sequence----------------//
	0x10, // Set SAP);BT[3:0]);AP);SLP);STB
	0x11, // Set DC1[2:0]);DC0[2:0]);VC
	0x12, // Set VREG1OUT voltage
	0x13, // Set VCOM AMP voltage
	0x07, // Set VCOM AMP voltage
	0x07, // Set VCOM AMP voltage
	0x00, // delay 200
	0x10, // Set SAP);BT[3:0]);AP);SLP);STB
	0x11, // Set DC1[2:0]);DC0[2:0]);VC[2:0]
	0x00, // delay 50
	0x12, // Set VREG1OUT voltaged
	0x00, // delay 50
	0x13, // Set VCOM AMP voltage
	0x29, // Set VCOMH voltage
	0x2B, // Set Frame rate.
	0x00, // delay 50
	0x20, // Set GRAM Horizontal Address
	0x21, // Set GRAM Vertical Address
	//****************************************************
	0x30,
	0x31,
	0x32,
	0x35,
	0x36,
	0x37,
	0x38,
	0x39,
	0x3C,
	0x3D,
	//---------------  RAM Address Control ----------------//
	0x50, // Set GRAM Horizontal Start Address
	0x51, // Set GRAM Horizontal End Address
	0x52, // Set GRAM Vertical Start Address
	0x53, // Set GRAM Vertical End Address
	//---------------  Panel Image Control -----------------//
	0x60, // Set Gate Scan line
	0x61, // Set NDL); VLE); REV
	0x6A, // Set Scrolling line
	//---------------  Panel Interface Control---------------//
	0x90,
	0x92,
	//--------------- Display On-------------------------------//
	0x07 // Display on		
};
const WORD DisplayConfData[48]=
{
	0x0100, // Set SS and SM bit
	0x0700, // Set line inversion
	#ifdef DISPLAY_WRITE_POS
	0x1010, // Set Write direction
	#else
	0x1291, // Set Write direction 
	#endif
	0x0000, // Set Scaling function off
	0x0207, // Set BP and FP
	0x0000, // Set non-display area
	0x0000, // Frame marker control
	0x0000, // Set interface control
	0x0000, // Frame marker Position
	0x0000, // Set RGB interface 
	//--------------- Power On Sequence----------------//
	0x0000, // Set SAP);BT[3:0]);AP);SLP);STB
	0x0007, // Set DC1[2:0]);DC0[2:0]);VC
	0x0000, // Set VREG1OUT voltage
	0x0000, // Set VCOM AMP voltage
	0x0001, // Set VCOM AMP voltage
	0x0020, // Set VCOM AMP voltage
		 200, // delay 200
	0x1290, // Set SAP);BT[3:0]);AP);SLP);STB
	0x0221, // Set DC1[2:0]);DC0[2:0]);VC[2:0]
		  50, // delay 200	
	0x0081, // Set VREG1OUT voltaged
			50, // delay 50
	0x1500, // Set VCOM AMP voltage
	0x000c, // Set VCOMH voltage
	0x000D, // Set Frame rate.
		  50, // delay 50
	0x0000, // Set GRAM Horizontal Address
	0x0000, // Set GRAM Vertical Address
	//****************************************************
	0x0303,
	0x0006,
	0x0001,
	0x0204,
	0x0004,
	0x0407,
	0x0000,
	0x0404,
	0x0402,
	0x0004,
	//---------------  RAM Address Control ----------------//
	0x0000, // Set GRAM Horizontal Start Address
	0x00EF, // Set GRAM Horizontal End Address
	0x0000, // Set GRAM Vertical Start Address
	0x013F, // Set GRAM Vertical End Address
	//---------------  Panel Image Control -----------------//
	0xA700, // Set Gate Scan line
	0x0001, // Set NDL); VLE); REV
	0x0000, // Set Scrolling line
	//---------------  Panel Interface Control---------------//
	0x0010,
	0x0000,
	//--------------- Display On-------------------------------//
	0x0133 // Display on
}; 


