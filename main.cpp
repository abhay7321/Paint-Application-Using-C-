#include "point.h"
#include "line.h"
#include "curve.h"
#include "polygon.h"

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <list>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <windows.h> // for MS Windows
#include <GL/glut.h> // GLUT, include glu.h and gl.h
#endif

using namespace std;
#define pi 3.142857

//Value of RGB while Drawing
float red = 0.0, green = 0.0, blue = 0.0;

int ptsize = 2;
int brushSize = 4;
int edge = 3;
int eraserSize = 1;
int tmpx, tmpy; // store the first point when shape is line, rectangle or circle
bool isSecond = false;
/*
When shape is line , rect , circle after storing the first point we make isSecond =true ......
so as to take new values of x , y and give call to line , or rect , or circle with 4 parameters
, old points stores in tmpx ,tmpy and new points stored in x, y
*/
bool isfilled = false;

bool isRandom = false; //When its true it selects random color
bool isEraser = false; //When we have selected Eraser

float window_w = 500;
float window_h = 500;
bool isRadial = false;

//Initially we have kept shape as point
int shape = 1; // 1:point, 2:line, 3:rectangle, 4:circle, 5:brush

vector<Point> points;		 // store all the points until clear
std::list<int> undoHistory;	 // record for undo, maximum 20 shapes in history
std::list<int> redoHistory;	 // record for redo, maximum 20 shapes in history
std::vector<Point> redoDots; // store the dots after undo temporaly

line L;
circle C;
Rectang R;
Polyg P1;

void printGuide();

void clear()
{
	points.clear();
	undoHistory.clear();
	redoDots.clear();
	redoHistory.clear();
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
	cout << "[Info] The window is cleared successfully.\n";
}

void quit()
{
	cout << "Thank you for using this Paint tool! Goodbye!" << endl;
	exit(0);
}

void undo()
{
	if (undoHistory.size() > 0)
	{
		if (undoHistory.back() != points.size() && redoHistory.back() != points.size())
		{
			redoHistory.push_back(points.size());
		}

		int numRemove = points.size() - undoHistory.back();

		for (int i = 0; i < numRemove; i++)
		{
			redoDots.push_back(points.back());
			points.pop_back();
		}
		redoHistory.push_back(undoHistory.back());
		undoHistory.pop_back();
	}
	else
	{
		cout << "[Warning] Cannot undo. This is the first record in the history.\n";
	}
}

void redo()
{
	if (redoHistory.size() > 1)
	{
		undoHistory.push_back(redoHistory.back());
		redoHistory.pop_back();
		int numRemove = redoHistory.back() - points.size();
		for (int i = 0; i < numRemove; i++)
		{
			points.push_back(redoDots.back());
			redoDots.pop_back();
		}
	}
	else
	{
		cout << "[Warning] Cannot redo. This is the last record in the history.\n";
	}
}

/*
FOR GLUT : mouse_x and mouse_y
Top left corner is the origin
As we move on right x increases
As we move down y increases


But For OPENGL
Bottom Left Corner is the origin
As we move on right x increases
As we move up y increases

*/
void drawPoint(int mousex, int mousey)
{
	Point newPoint(mousex, window_h - mousey, isEraser ? 1.0 : red, isEraser ? 1.0 : green, isEraser ? 1.0 : blue);
	points.push_back(newPoint);

	/*
Its taking in 2 parameters a)mouse_x, mouse_y....
red ,green ,blue are global variables.........
isEraser tell weather we have selected eraser or not......
Becoz if eraser is selected then value of rgb at that point must be(1,1,1)..

So it simply sees the point and color of the point
Makes a new object of class POINT........Initializes the object by seeing the values........ and pushes the POINT object in dots vector.............
*/
}

void line::drawLine(int x1, int y1, int x2, int y2)
{
	if (x1 == x2)
	{
		for (float i = (y1 < y2 ? y1 : y2); i <= (y1 > y2 ? y1 : y2); i += 0.01)
		{
			drawPoint(x1, i);
		}
		return;
	}
	else if (y1 == y2)
	{
		for (float i = (x1 < x2 ? x1 : x2); i <= (x1 > x2 ? x1 : x2); i += 0.01)
		{
			drawPoint(i, y1);
		}
		return;
	}
	else
	{
		for (float t = 0; t <= 1; t += .001)
		{
			float x = x1 + (x2 - x1) * t;
			float y = y1 + (y2 - y1) * t;
			drawPoint(x, y);
		}
	}
	return;
}

//x1,y1 and x2,y2 must be any 2 diagonal points of rectangle
void Rectang ::drawRectangle(int x1, int y1, int x2, int y2)
{
	drawLine(x1, y1, x2, y1);
	drawLine(x2, y1, x2, y2);
	drawLine(x2, y2, x1, y2);
	drawLine(x1, y2, x1, y1);
}

/*
void drawRectangle_filled(int x1, int y1, int x2, int y2)
{
    if(y1>y2)
    {
        for(float i=y2 ; i<=y1 ;i++)
        {
            L.drawLine(x1, i, x2, i);
        }
    }
    else
    {
        for(float i=y1 ; i<=y2 ;i++)
        {
            L.drawLine(x1, i, x2, i);
        }
    }
}
*/

void circle::drawCircle(int x1, int y1, int x2, int y2)
{
	float r = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	float x, y;

	for (float i = 0; i < (2 * pi); i += 0.001)
	{
		//Using Parametric Equation of circle
		// circle is defined as x=r*cos(i) and y=r*sin(i)
		x = x1 + r * cos(i);
		y = y1 + r * sin(i);
		drawPoint(x, y);
	}
}
/*
void drawCircle_Filled(int x1, int y1, int x2, int y2)
{
    float r = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    float x,y;


    for(float j=0 ; j<=r ; j++)
    {
        for (float i = 0; i < (2 * pi); i += 0.001)
        {
            //Using Parametric Equation of circle
            // circle is defined as x=r*cos(i) and y=r*sin(i)
            x = x1 + j * cos(i);
            y = y1 + j * sin(i);
            drawPoint(x, y);
        }
    }
}
*/

void Polyg::draw_polygon(int x1, int y1, int x2, int y2)
{
	float x, y, m, n, i;
	x = x1;
	y = y1;
	float a = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	float interior = (pi * (edge - 2) / edge);
	float k = pi - interior;
	//Writing in general function{}
	for (int j = 0; j < edge; j++)
	{
		m = x;
		n = y;
		L.drawLine(m, n, m + a * cos(k * j), n + a * sin(k * j));
		x = m + a * cos(k * j);
		y = n + a * sin(k * j);
	}
}

/*
//It draws a dot of white color , wherever we want to erase
//if eraser size is 1 , then it will erase  square of 3*3
//if eraser size is 2 , then it will erase  square of 5*5
//if eraser size is x , then it will erase  square of (2x+1)*(2x+1)
*/
void erase(int x, int y)
{
	for (int i = -eraserSize; i <= eraserSize; i++)
	{
		for (int j = -eraserSize; j <= eraserSize; j++)
		{
			drawPoint(x + i, y + j);
		}
	}
}

void drawRadialBrush(int x, int y)
{
	int xc = glutGet(GLUT_WINDOW_WIDTH) / 2;
	int yc = glutGet(GLUT_WINDOW_HEIGHT) / 2;
	int dx, dy;

	dx = xc - x;
	dy = yc - y;

	/*
	Taking img wrt x-axis
	Taking img wrt y-axis
	Taking img wrt origin => the centre of the screen

	And then interchanging x and y for all these 4 points
	*/
	drawPoint(xc + dx, yc + dy);
	drawPoint(xc - dx, yc + dy);
	drawPoint(xc + dx, yc - dy);
	drawPoint(xc - dx, yc - dy);

	drawPoint(xc + dy, yc + dx);
	drawPoint(xc - dy, yc + dx);
	drawPoint(xc + dy, yc - dx);
	drawPoint(xc - dy, yc - dx);
}

/*
Same as draw dot
//It actually gives a call to drawDot Function
//The difference is , drawBrush is used for AirBrush
//So when size of AirBrush is 4px , it puts 4 points very near to our selected (x,y)
//So when size of AirBrush is 8px , it puts 8 points very near to our selected (x,y)
//So when size of AirBrush is 12px , it puts 12 points very near to our selected (x,y)
//So when size of AirBrush is 16px , it puts 16 points very near to our selected (x,y)
//Its using rand() to get a random point very near to our point
*/
void drawBrush(int x, int y)
{
	for (int i = 0; i < brushSize; i++)
	{
		int randX = rand() % (brushSize + 1) - brushSize / 2 + x;
		int randY = rand() % (brushSize + 1) - brushSize / 2 + y;
		drawPoint(randX, randY);
	}
	/*
	Larger the Size of the Brush , More the Nos of Dots it prints in one Click
	Also how long the dots would be printed from the position of our click , would again depend on size of the brush...   % (brushSize + 1)
	If size of brush .., width would be more else the width would be less......   rand() % (brushSize + 1) ..when brushSize is more ,ts value would also be more

	The C library function int rand(void) returns a pseudo-random number in the range of 0 to RAND_MAX.
    RAND_MAX is a constant whose default value may vary between implementations but it is granted to be at least 32767.


	Due to this rand() % (brushSize + 1) will always be positive ...Hence the RanDom Points Will Only be on One Side of Our pt. X,Y....
	//To avoid this we are doing - brushSize / 2.....so the points would be evenly distributed around the point where we clicked
    */
}



//GLUTMOTIONFUNC gives call to The motion , when the mouse moves within the window while one or more mouse buttons are pressed.
void motion(int x, int y)
{
	if (isEraser)
		erase(x, y);
	else
	{
		if (shape == 1)
		{
			if (isRadial)
				drawRadialBrush(x, y);
			else
				drawPoint(x, y);
		}
		if (shape == 5)
			drawBrush(x, y);
	}
}

//When a user presses and releases mouse buttons in the window , glutMouseFunc sets the mouse callback for the current window.
void mouse(int bin, int state, int x, int y)
{
	if (bin == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (isRandom)
		{
			red = float(rand()) / float(RAND_MAX);
			green = float(rand()) / float(RAND_MAX);
			blue = float(rand()) / float(RAND_MAX);
		}

		if (isEraser)
		{
			undoHistory.push_back(points.size());
			erase(x, y);
		}
		else
		{
			if (shape == 1)
			{
				undoHistory.push_back(points.size());
				if (isRadial)
					drawRadialBrush(x, y);
				else
					drawPoint(x, y);
			}
			else if (shape == 5)
			{
				undoHistory.push_back(points.size());
				drawBrush(x, y);
			}
			else
			{
				if (!isSecond)
				{
					tmpx = x;
					tmpy = y;
					isSecond = true;
				}
				else
				{
					if (undoHistory.back() != points.size())
						undoHistory.push_back(points.size());
					if (shape == 2)
						L.drawLine(tmpx, tmpy, x, y);
					else if (shape == 3)
						R.drawRectangle(tmpx, tmpy, x, y);
					else if (shape == 4)
						C.drawCircle(tmpx, tmpy, x, y);
					else if (shape == 6)
						P1.draw_polygon(tmpx, tmpy, x, y);
					isSecond = false;
				}
			}
		}

		if (undoHistory.size() > 20)
		{
			undoHistory.pop_front();
		}
	}
}

void processColourMenu(int value)
{
	isSecond = false;
	isEraser = false;
	isRandom = false;

	switch (value)
	{
	case 1: // red
		red = 1.0;
		green = 0.0;
		blue = 0.0;
		break;
	case 2: // green
		red = 0.0;
		green = 1.0;
		blue = 0.0;
		break;
	case 3: // blue
		red = 0.0;
		green = 0.0;
		blue = 1.0;
		break;
	case 4: // purple
		red = 0.5;
		green = 0.0;
		blue = 0.5;
		break;
	case 5: // yellow
		red = 1.0;
		green = 1.0;
		blue = 0.0;
		break;
	case 6: // black
		red = 0.0;
		green = 0.0;
		blue = 0.0;
		break;
	case 7: // random
		isRandom = true;
		break;
	}
}

void processBrushSizeMenu(int value)
{
	shape = 5;
	isEraser = false;
	brushSize = value;
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
}

void processShapeMenu(int value)
{
	shape = value;
	isEraser = false;
	isSecond = false;

	switch (shape)
	{
	case 1:
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
		break;
	case 2:
	case 3:
	case 4:
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		break;
	}
}

void processBorderMenu(int value)
{
	isSecond = false;
	isEraser = false;
	isRandom = false;

	ptsize = value;
}

void processEraserSizeMenu(int value)
{
	glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	eraserSize = value;
	isEraser = true;
}

void processEdgeMenu(int value)
{
	shape = 6;
	isEraser = false;
	isSecond = false;
	edge = value;
}

void processFilledMenu(int value)
{
	shape = 3;
	isEraser = false;
	isSecond = false;
	if (value)
		isfilled = true;
	else
		isfilled = false;
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
}

void processMainMenu(int value)
{
	switch (value)
	{
	case 0:
		quit();
		break;
	case 1:
		clear();
		break;

	case 2:
		undo();
		break;
	case 3:
		redo();
		break;
	}
}

void processRadicalBrushMenu(int value)
{
	isRadial = value == 1 ? true : false;
}

void createOurMenu()
{
	/*
    int glutCreateMenu(void (*func)(int value));
    //glutCreateMenu creates a new pop-up menu.
func
The callback function for the menu is called when a menu entry from the menu is selected.
The value passed to the callback is determined by the value for the selected menu entry.
    */
	int colourMenu = glutCreateMenu(processColourMenu); //Creates a Pop-Up Menu
	glutAddMenuEntry("Red", 1);
	/*
	  glutAddMenuEntry adds a menu entry to the bottom of the current menu.

        void glutAddMenuEntry(char *name, int value);
        name    :      ASCII character string to display in the menu entry.
        value   :      Value to return to the menu's callback function if the menu entry is selected.
        Description     :       glutAddMenuEntry adds a menu entry to the bottom of the current menu. The string name will be displayed for the newly added menu entry.
        If the menu entry is selected by the user,  the menu's callback will be called passing value as the callback's parameter
	*/
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("Purple", 4);
	glutAddMenuEntry("Yellow", 5);
	glutAddMenuEntry("Black", 6);
	glutAddMenuEntry("Random", 7);

	int sizeMenu = glutCreateMenu(processBrushSizeMenu);
	glutAddMenuEntry("4px", 4);
	glutAddMenuEntry("8px", 8);
	glutAddMenuEntry("12px", 12);
	glutAddMenuEntry("16px", 16);

	int sideMenu = glutCreateMenu(processEdgeMenu);
	glutAddMenuEntry("3", 3);
	glutAddMenuEntry("4", 4);
	glutAddMenuEntry("5", 5);
	glutAddMenuEntry("6", 6);
	glutAddMenuEntry("7", 7);
	glutAddMenuEntry("8", 8);
	glutAddMenuEntry("9", 9);
	glutAddMenuEntry("10", 10);
	glutAddMenuEntry("11", 11);
	glutAddMenuEntry("12", 12);
	glutAddMenuEntry("15", 15);
	glutAddMenuEntry("16", 16);
	glutAddMenuEntry("20", 20);

	int filledMenu = glutCreateMenu(processFilledMenu);
	glutAddMenuEntry("Fill_Area", 1);
	glutAddMenuEntry("Unfill_Area", 0);

	int shapeMenu = glutCreateMenu(processShapeMenu);
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Line", 2);
	glutAddSubMenu("Rectangle", filledMenu);
	glutAddMenuEntry("Circle", 4);
	glutAddSubMenu("Airbrush", sizeMenu);
	glutAddSubMenu("Polygon", sideMenu);

	int borderMenu = glutCreateMenu(processBorderMenu);
	glutAddMenuEntry("2px", 2);
	glutAddMenuEntry("4px", 4);
	glutAddMenuEntry("5px", 5);
	glutAddMenuEntry("6px", 6);

	int eraserSizeMenu = glutCreateMenu(processEraserSizeMenu);
	glutAddMenuEntry("Small", 2);
	glutAddMenuEntry("Medium", 6);
	glutAddMenuEntry("Large", 10);

	int radicalBrushMenu = glutCreateMenu(processRadicalBrushMenu);
	glutAddMenuEntry("True", 1);
	glutAddMenuEntry("False", 2);

	glutCreateMenu(processMainMenu);
	glutAddSubMenu("Colour", colourMenu);
	glutAddSubMenu("Shapes", shapeMenu);
	glutAddSubMenu("Border", borderMenu);
	glutAddSubMenu("Eraser", eraserSizeMenu);
	glutAddSubMenu("Radical Paint Brush", radicalBrushMenu);
	glutAddMenuEntry("Undo", 2);
	glutAddMenuEntry("Redo", 3);
	glutAddMenuEntry("Clear", 1);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/*
	glutAttachMenu attaches a mouse button for the current window to the identifier of the current menu;
	glutDetachMenu detaches an attached mouse button from the current window.
	By attaching a menu identifier to a button, the named menu will be popped up when the user presses the specified button.
	button should be one of GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, and GLUT_RIGHT_BUTTON.
	Note that the menu is attached to the button by identifier, not by reference.
	*/
	/*
        glutAddSubMenu adds a sub-menu trigger to the bottom of the current menu.

void glutAddSubMenu(char *name, int menu);
name    :   ASCII character string to display in the menu item from which to cascade the sub-menu.
menu    :   Identifier of the menu to cascade from this sub-menu menu item.

Description

glutAddSubMenu adds a sub-menu trigger to the bottom of the current menu.
The string name will be displayed for the newly added sub-menu trigger.
If the sub-menu trigger is entered, the sub-menu numbered menu will be cascaded, allowing sub-menu menu items to be selected.
    */
}

//Event HandLing Zone

void display(void)
{
	/*
    After we made some changes in our window ,they must be immediately displayed  ,hence this function would be called
    Before calling it , it displayed the last time window , but now since we have made changes , many points have been drawn and many are removed ...
    Hence to reflect those changes .....we first clear whatever is shown on the window and then again print all the dots in the entire vector
    The alpha parameter is a number between 0.0 (fully transparent) and 1.0 (fully opaque).
    */

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // specifies the red, green, blue, and alpha values used by glClear to clear the color buffers.
	glClear(GL_COLOR_BUFFER_BIT);		  //clear buffers to preset values

	glPointSize(ptsize); //Setting the size of the point
	/*
    Ten symbolic constants are accepted: GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS, GL_QUAD_STRIP, and GL_POLYGON.
    */

	glBegin(GL_POINTS);
	for (unsigned int i = 0; i < points.size(); i++)
	{
		glColor3f(points[i].getR(), points[i].getG(), points[i].getB()); //set the current color
		glVertex2i(points[i].getX(), points[i].getY());					 //To draw the point
	}
	glEnd();

	glutSwapBuffers();
	/*
	"Buffer" here refers to the region of (video) memory where the computer renders image frames.

With double-buffering, two such buffers are used, the front buffer and the back buffer. The front buffer is the one that contains the frame you are currently seeing. The back buffer is the one in which the computer is currently busy rendering the next frame. When that rendering is done, the two buffers are swapped, instantly updating the image you see to be the next-now-current frame. The computer then again moves on to rendering a new frame in what is now its current back buffer.

This technique ensures that you only ever see fully rendered frames rather than "work in progress".

glutSwapBuffers() is the GLUT function that instructs the computer that you are done with the current frame and the buffers should be swapped so that that frame be displayed and so that you can begin working on the next.
	*/
}

void reshape(int w, int h)
{
	window_w = w;
	window_h = h;

	//Before Calling gluOrtho2D we need to have the below 2 lines of code
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Rectangular Region inWorld to be displayed
	gluOrtho2D(0, w, 0, h); //( width_start , height start ,width end , height end )

	//These Functions can be Used Later if we Want to implement Functionalities like ZoomIn and ZoomOut

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h); // Area on Screen to Display Objects ( width_start , height start ,width end , height end )
}

void keyboard(unsigned char key, int xIn, int yIn)
{
	isSecond = false;
	switch (key)
	{
	//Come Out From the Paint Application when we press (q or escape)
	case 'q':
	case 27: // 27 is the esc key
		quit();
		break;
	case 'c':
		clear();
		break;
	case '+':
		if (shape == 5 && !isEraser)
		{
			if (brushSize < 16)
				brushSize += 4;
			else
			{
				cout << "[Warning] Airbrush's size cannot be larger. It is already the largest.\n";
			}
		}
		else if (isEraser)
		{
			if (eraserSize < 10)
				eraserSize += 4;
			else
			{
				cout << "[Warning] Eraser's size cannot be larger. It is already the largest.\n";
			}
		}
		break;
	case '-':
		if (shape == 5 && !isEraser)
		{
			if (brushSize > 4)
				brushSize -= 4;
			else
			{
				cout << "[Warning] Airbrush's size cannot be smaller. It is already the smallest.\n";
			}
		}
		else if (isEraser)
		{
			if (eraserSize > 2)
				eraserSize -= 4;
			else
			{
				cout << "[Warning] Eraser's size cannot be smaller. It is already the smallest.\n";
			}
		}
		break;
	case 'u':
		undo();
		break;
	case 'r':
		redo();
		break;
	}
}

void Timer(int val)
{
	glutPostRedisplay();		//To give window paint request which in turn activate display function.
	glutTimerFunc(0, Timer, 0); //Timer function will be called back after every specific interval.
}

void callbackInit()
{
	//glutDisplayFunc is called whenever your window must be redrawn.
	glutDisplayFunc(display);

	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);
	/*
	Sets the Keyboard callback for the current window.
	glutKeyboardFunc(void( * callback )( unsigned char key, int x, int y ));
	callback Client function for keyboard event.


    As a convenience, the mouse coordinates, relative to your window, are also returned.
    This callback is bound to the current window .
	*/

	glutMouseFunc(mouse);
	/*
	glutMouseFunc sets the mouse callback for the current window. When a user presses and releases mouse buttons in the window,
	each press and each release generates a mouse callback. 
	The button parameter is one of GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON.
    The state parameter is either GLUT_UP or GLUT_DOWN indicating whether the callback was due to a release or press respectively.
    The x and y callback parameters indicate the window relative coordinates when the mouse button state changed. 

    If a menu is attached to a button for a window, mouse callbacks will not be generated for that button.

	*/
	glutMotionFunc(motion);
	/*

glutMotionFunc and glutPassiveMotionFunc set the motion and passive motion callback respectively for the current window.
The motion callback for a window is called when the mouse moves within the window while one or more mouse buttons are pressed.
The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.
The x and y callback parameters indicate the mouse location in window relative coordinates.
Passing NULL to glutMotionFunc or glutPassiveMotionFunc disables the generation of the mouse or passive motion callback respectively.
*/

	glutTimerFunc(0, Timer, 0);
}

int main(int argc, char **argv)
{
	//glutInit will initialize the GLUT library and negotiate a session with the window system.
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	/*
	GLUT_RGBA
    Bit mask to select an RGBA mode window. This is the default if neither GLUT_RGBA nor GLUT_INDEX are specified.

    GLUT_DOUBLE
    Bit mask to select a double buffered window. This overrides GLUT_SINGLE if it is also specified.
    */

	//Setting Size and position of the Window
	glutInitWindowSize(window_w, window_h);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Paint"); //Giving name to our Window

	callbackInit();

	//To print all the instructions
	printGuide();

	//creating Our Graphical User Interface
	createOurMenu();

	//Goes in an infinite loop till we give call to exit(0) and in this loop gives call to display function
	glutMainLoop();

	return (0);
}

void printGuide()
{
	std::cout << "#########################################################################\n"
			  << "#                    Welcome to use this Paint tool!                    #\n"
			  << "#########################################################################\n"
			  << "A list of commands:\n"
			  << "Right click\t"
			  << "-> show menu\n"
			  << "Left click\t"
			  << "-> choose option\n"
			  << "Menu \"Color\"\t"
			  << "-> You can choose Red, Green, Blue, Yellow or Random, the default color is Red.\n"
			  << "Menu \"Shapes\"\t"
			  << "-> The default shape is Point.\n"
			  << "\tPoint\t\t"
			  << "-> draw a dot at the point clicked with the mouse. Clicking and dragging will draw points constantly like free-form drawing.\n"
			  << "\tLine\t\t"
			  << "-> draw a line between two subsequently clicked points.\n"
			  << "\tRectangle\t"
			  << "-> draw a rectangle with top-left corner specified by the first click and the bottom-right corner specified by a second click. If the second click is bottom-left, top-right or top-left comparing to the first click, a warning will show in the console.\n"
			  << "\tCircle\t\t"
			  << "-> draw a circle centered at the position of the first click, with its radius set by a second click.\n"
			  << "\tAirbrush\t"
			  << "-> draw multiple points as brush around the clicked point. There are four options of size.\n"
			  << "Menu \"Eraser\"\t"
			  << "-> erase the points by clicking and dragging.\n"
			  << "Menu \"Clear\"\t"
			  << "-> clear all the points\n"
			  << "Menu \"Quit\"\t"
			  << "-> close the window.\n"
			  << "Keyboard 'q'\t"
			  << "-> close the window.\n"
			  << "Keyboard 'esc'\t"
			  << "-> close the window.\n"
			  << "Keyboard 'c'\t"
			  << "-> clear all the points and clear the history.\n"
			  << "Keyboard '+'\t"
			  << "-> larger size of eraser or brush.\n"
			  << "Keyboard '-'\t"
			  << "-> smaller size of eraser or brush.\n"
			  << "################################# Paint #################################" << std::endl;
}
