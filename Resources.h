# define IDI_REG_ICON				100
# define ICON_REG_PATH				"Resources\\Reg.ico"
# define IDD_MAINDIALOG				9001
# define IDC_LISTVIEW				101
# define IDC_STATIC_INFO			51
# define IDC_BUTTON_SELECT			201
# define IDC_BUTTON_CANCEL			203

# define S_CAPTION					"Registration"
# define S_SELECT					"Select"
# define S_CANCEL					"Cancel"
# define S_INFO						"Choose a Disk to register with:"

# define WINDOW_X					0
# define WINDOW_Y					0
# define WINDOW_WIDTH				300
# define WINDOW_HEIGHT				120

# define BORDER_SIZE				2

# define INFO_X						0
# define INFO_Y						0
# define INFO_WIDTH					WINDOW_WIDTH
# define INFO_HEIGHT				18

# define LIST_VIEW_X				BORDER_SIZE
# define LIST_VIEW_Y				INFO_Y+INFO_HEIGHT
# define LIST_VIEW_WIDTH			WINDOW_WIDTH-BORDER_SIZE-BORDER_SIZE
# define LIST_VIEW_HEIGHT			74

# define BUTTON_WIDTH				50
# define BUTTON_HEIGHT				20

# define OPTIONS_GAP				10
# define OPTIONS_Y					LIST_VIEW_Y+LIST_VIEW_HEIGHT+3

# define SELECT_X					98
//(WINDOW_WIDTH-2*BORDER_WIDTH)/2-(BUTTON_WIDTH*2+BUTTON_GAP)/2;
# define SELECT_Y					OPTIONS_Y
# define SELECT_WIDTH				BUTTON_WIDTH
# define SELECT_HEIGHT				BUTTON_HEIGHT

# define CANCEL_X					SELECT_X+SELECT_WIDTH+OPTIONS_GAP
# define CANCEL_Y					OPTIONS_Y
# define CANCEL_WIDTH				BUTTON_WIDTH
# define CANCEL_HEIGHT				BUTTON_HEIGHT

# define FONT_SIZE					8
# define FONT_NAME					"MS Sans Serif"
