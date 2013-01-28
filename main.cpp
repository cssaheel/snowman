#include <windows.h> 
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif   
#include <stdio.h>
#include <ctime> 
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <time.h>
//#include <GL/glut.h>     
//#include<math.h>
using namespace std;
int safeMove=0;
int status=0; //0: none, 1:won, 2: lose
float px=0.0,py=0.0;
float deltaAngle = -3.2;
float deltaMove = 0.26;
int initialized=1;
float ratio=1.4f;
int width,height;
float border=3.65;
float angle = 0.0f;
int pd=4; // 1: UP, 2: DOWN, 3: LEFT, 4: RIGHT
int ppd=4; // previous pd
float cubesx[16],cubesy[16],cubesxm[16],cubesym[16],pcx[16],pcy[16],damage[16],damagex,damagey,ghostAngle[4],ghostMove[4],ghost[4],gx[4],gy[4],gd[4];
int boolr=0;
int booll=0;
int boold=0;
int boolu=0;
 GLfloat redDiffuseMaterial[] = {1.0, 0.0, 0.0}; //set the material to red
 GLfloat whiteSpecularLight[] = {1.0, 1.0, 1.0}; //set the light specular to white
GLfloat blackAmbientLight[] = {0.0, 0.0, 0.0}; //set the light ambient to black
GLfloat whiteDiffuseLight[] = {1.0, 1.0, 1.0}; //set the diffuse light to white


void drawBitmapText(char *string,float x,float y,float z) 
{  
	glColor3f(1,0,0);
	char *c;
	glRasterPos3f(x, y,z);

	for (c=string; *c != '\0'; c++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

void light (void) {
    //glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
    //glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redDiffuseMaterial);
}

void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	
	width = w;
	height = h;

	ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}
int init0=0;
void drawGhost(){
// Draw Head
	//srand(clock());
	if(init0==0){
		init0=1;
		for(int g=0;g<4;g++){
			int place=0;
			while(place==0){
			int sig=rand()%2;
			int x=rand()%35;
			float xf=x/10;
			if(sig==1)
				xf=-xf;
			int y=rand()%35;
			float yf=y/10;
			sig=rand()%2;
			if(sig==1)
				yf=-yf;
			ghostAngle[g]=xf;
		    ghostMove[g]=yf;
			place=0;
			for(int k=0;k<16;k++){
			if(ghostAngle[g]>cubesx[k] && ghostAngle[g]-cubesx[k]>-0.7 && ghostAngle[g]-cubesx[k]<0.7 && ghostMove[g]-cubesy[k]>-0.7 && ghostMove[g]-cubesy[k]<0.7 && damage[k]!=3)
				break;
			if(ghostAngle[g]<cubesx[k] && ghostAngle[g]-cubesx[k]>-0.7 && ghostAngle[g]-cubesx[k]<0.7 && ghostMove[g]-cubesy[k]>-0.7 && ghostMove[g]-cubesy[k]<0.7 && damage[k]!=3)
				break;
			if(ghostMove[g]>cubesy[k] && ghostMove[g]-cubesy[k]>-0.7 && ghostMove[g]-cubesy[k]<0.7 && ghostMove[g]-cubesy[k]>-0.7 && ghostMove[g]-cubesy[k]<0.7 && damage[k]!=3)
				break;
			if(ghostMove[g]-cubesy[k]>-0.7 && ghostMove[g]-cubesy[k]<0.7 && ghostMove[g]-cubesy[k]>-0.7 && ghostMove[g]-cubesy[k]<0.7 && damage[k]!=3)
				break;
			if(k==15)
				place=1;
			}
			}
		}
	}

	int i,z,max=0;
	float temp=0.0,maxVal=0.0;
	
	
	for(z=0;z<4;z++){
		glColor3f(1.0f,0.0f,0.0f);
		glPushMatrix();
		
	if(gx[z]<0){
		ghostAngle[z]-=0.001;
		gx[z]+=0.001;
		if(gx[z]==0){
			int temp=rand();
			gx[z]=(temp%7);
		}
	} else if(gx[z]>0){
		ghostAngle[z]+=0.001;
		gx[z]-=0.001;
		if(gx[z]==0){
			int temp=rand();
			gx[z]=-(temp%7);
		}
	}
		if(gy[z]<0){
		ghostMove[z]-=0.001;
		gy[z]+=0.001;
		if(gy[z]==0){
			int temp=rand();
			gy[z]=(temp%7);
		}
	    } else if(gy[z]>0){
			ghostMove[z]+=0.001;
			gy[z]-=0.001;
			if(gy[z]==0){
				int temp=rand();
				gy[z]=-(temp%7);
		}
		}
		/*
		if(ghostAngle[z]<0.0 && gx[z]!=0.0){
			ghostAngle[z]-=0.001;
			gx[z]+=0.001;
		} else if(ghostAngle[z]>0.0 && gx[z]!=0.0){
			ghostAngle[z]+=0.001;
			gx[z]-=0.001;
		}
		if(ghostMove[z]<0.0 && gy[z]!=0.0){
			ghostMove[z]-=0.001;
			gy[z]+=0.001;
		} else if(ghostMove[z]>0.0 && gy[z]!=0.0){
			ghostMove[z]+=0.001;
			gy[z]-=0.001;
		}
		*/
	if(ghostAngle[z]<-3.65){
	ghostAngle[z]=-3.6;
	gx[z]=-gx[z];
	//gy=(gy+rand(ctime())+ctime());
	gd[z]=4;
	} else if(ghostAngle[z]>3.65){
	ghostAngle[z]=3.6;
	gx[z]=-gx[z];
	//gy=-(gy+rand(ctime())+ctime());
	gd[z]=3;
	} 
	if(ghostMove[z]<-3.65){
	ghostMove[z]=-3.6;
	//gx=(gx+rand(ctime())+ctime());
	gy[z]=-gy[z];
	gd[z]=1;
	} else if(ghostMove[z]>3.65){
	ghostMove[z]=3.6;
	//gx=-(gx+rand(ctime())+ctime());
	gy[z]=-gy[z];
	gd[z]=2;
	}
	srand(clock());
	for(i=15;i>=0;i--){
	//if(ghostAngle[z]<cubesx[i] && ghostAngle[z]+0.001>=cubesx[i]-0.7 && cubesy[i]-ghostMove[z]>-0.3 && cubesy[i]-ghostMove[z]<0.3 && gd[z]==4 && damage[i]!=3){
		if(ghostAngle[z]>cubesx[i] && ghostAngle[z]-cubesx[i]>-0.7 && ghostAngle[z]-cubesx[i]<0.7 && ghostMove[z]-cubesy[i]>-0.7 && ghostMove[z]-cubesy[i]<0.7 && damage[i]!=3){
			gx[z]=(gx[z]+rand()%3);
			//if(ghostMove[z]>cubesy[i])
				gy[z]=-(gy[z]+rand()%3);
			//gy[z]=-(gy[z]+rand()%3);
			//gy=-(gy+rand(ctime())+ctime());
			gd[z]=3;
			if(cubesxm[i]!=0 || cubesym[i]!=0)
				ghost[z]=1;

	}
	//else if(ghostAngle[z]>cubesx[i] && ghostAngle[z]-0.001<=cubesx[i]+0.7 && cubesy[i]-ghostMove[z]>-0.3 && cubesy[i]-ghostMove[z]<0.3 && gd[z]==3 && damage[i]!=3){
		if(ghostAngle[z]<cubesx[i] && ghostAngle[z]-cubesx[i]>-0.7 && ghostAngle[z]-cubesx[i]<0.7 && ghostMove[z]-cubesy[i]>-0.7 && ghostMove[z]-cubesy[i]<0.7 && damage[i]!=3){	
		gx[z]=-(gx[z]+rand()%3);
		gy[z]=-(gy[z]+rand()%3);
		//gy[z]=(gy[z]+rand()%3);
			//gy=-(gy+rand(ctime())+ctime());
			gd[z]=4;
			if(cubesxm[i]!=0 || cubesym[i]!=0)
				ghost[z]=1;
	}
	//else if(ghostMove[z]>cubesy[i] && ghostMove[z]-0.001<=cubesy[i]+0.7 && ghostAngle[z]-cubesx[i]>-0.3 && ghostAngle[z]-cubesx[i]<0.3 && gd[z]==2 && damage[i]!=3){
		if(ghostMove[z]>cubesy[i] && ghostMove[z]-cubesy[i]>-0.7 && ghostMove[z]-cubesy[i]<0.7 && ghostMove[z]-cubesy[i]>-0.7 && ghostMove[z]-cubesy[i]<0.7 && damage[i]!=3){	
			//gx=-(gx+rand(ctime())+ctime());
			//gx[z]=-(gx[z]+rand()%3);
			gy[z]=(gy[z]+rand()%3);
			gx[z]=-(gx[z]+rand()%3);
			gd[z]=1;
			if(cubesxm[i]!=0 || cubesym[i]!=0)
				ghost[z]=1;
	}
	//else if(ghostMove[z]<cubesy[i] && ghostMove[z]+0.001>=cubesy[i]-0.7 && ghostAngle[z]-cubesx[i]>-0.3 && ghostAngle[z]-cubesx[i]<0.3 && gd[z]==1 && ghostMove[z]-cubesy[i]>-0.7 && ghostMove[z]-cubesy[i]<0.7 && damage[i]!=3){
			if(ghostMove[z]-cubesy[i]>-0.7 && ghostMove[z]-cubesy[i]<0.7 && ghostMove[z]-cubesy[i]>-0.7 && ghostMove[z]-cubesy[i]<0.7 && damage[i]!=3){	
		//gx=-(gx+rand(ctime())+ctime());
				//gx[z]=(gx[z]+rand()%3);
			gy[z]=-(gy[z]+rand());
			gx[z]=-(gx[z]+rand()%3);
			gd[z]=2;
			if(cubesxm[i]!=0 || cubesym[i]!=0)
				ghost[z]=1;
	}
	}
	if(ghostAngle[z]-deltaAngle>-0.3 && ghostAngle[z]-deltaAngle<0.3 && ghostMove[z]-deltaMove>-0.3 && ghostMove[z]-deltaMove<0.3){
				if(ghost[z]==0)
					//exit(0);
					status=2; // you lose

	}
	if(ghost[0]==1 && ghost[1]==1 && ghost[2]==1 && ghost[3]==1)
		//exit(0);
		status=1; // you won

	glTranslatef(ghostAngle[z], ghostMove[z], 0.0f);
	// up
	if(gd[z]==1){
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glRotatef(-120, 1.0f, 0.0f, 0.0f);
	}
	// left
	if(gd[z]==3){
	glRotatef(-180, 0.0f, 1.0f, 0.0f);
	}
	// down
	if(gd[z]==2){
	glRotatef(-90, 0.0f, 0.0f, 1.0f);
	glRotatef(20, 1.0f, 0.0f, 0.0f);
	}
	if(ghost[z]!=1)
	glutSolidSphere(0.25,10,10);
	/*
	int nu=rand(),temp;
	temp=nu%10;
	gx[z]=(float)(temp)/10;
	nu=rand();
	temp=nu%10;
	gy[z]=(float)(temp)/10;
	*/

// Draw Eyes
	glPushMatrix();
	glColor3f(1.0f,1.0f,1.0f);
	glTranslatef(0.2f, 0.10f, 0.18f);
	glRotatef(90,0.0f, 1.0f, 0.0f);
	if(ghost[z]!=1)
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	if(ghost[z]!=1)
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

// Draw Nose
	glPushMatrix();
	glColor3f(1.0f, 0.5f , 0.5f);
	glTranslatef(0.24f, 0.0f, 0.0f);
	glRotatef(90,0.0f, 1.0f, 0.0f);
	if(ghost[z]!=1)
	glutSolidCone(0.07f,0.3f,10,2);
	glPopMatrix();
	glPopMatrix();
}
}


void drawPlayer(){
// Draw Head
	int i,max=0;
	float temp=0.0,maxVal=0.0;
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	/*
		for(i=15;i>=0;i--){
		if(deltaAngle>=cubesx[i]-0.8 && (deltaMove<=cubesy[i]+0.3 && deltaMove>=cubesy[i]-0.3) && pd==4)
			boolr=1;
		if(deltaAngle<=cubesx[i]+0.8 && (deltaMove<=cubesy[i]+0.3 && deltaMove>=cubesy[i]-0.3) && pd==3)
			booll=1;
		if(deltaMove>=cubesy[i]-0.5 && (deltaAngle<=cubesx[i]+0.3 && deltaAngle>=cubesx[i]-0.3) && pd==2)
			boold=1;
		if(deltaMove<=cubesy[i]+0.5 && (deltaAngle<=cubesx[i]+0.3 && deltaAngle>=cubesx[i]-0.3) && pd==1)
			boolu=1;
		}
		*/
			/*
			temp=cubesx[i]-deltaAngle;
			if(temp<0.5){
				max=i;
			    maxVal=cubesx[i];
			}
			//deltaAngle=cubesx[max]-0.5;
			deltaAngle-=0.4;
			break;
			*/
		if(px<0.0 && px!=0.0){
			deltaAngle-=0.001;
			px+=0.001;
		} else if(px>0.0 && px!=0.0){
			deltaAngle+=0.001;
			px-=0.001;
		}
		if(py<0.0 && py!=0.0){
			deltaMove-=0.001;
			py+=0.001;
		} else if(py>0.0 && py!=0.0){
			deltaMove+=0.001;
			py-=0.001;
		}
	if(deltaAngle<-3.65)
	deltaAngle=-3.65;
	if(deltaAngle>3.65)
	deltaAngle=3.65;
	if(deltaMove<-3.65)
	deltaMove=-3.65;
	if(deltaMove>3.65)
	deltaMove=3.65;
	
	for(i=15;i>=0;i--){
	if(deltaAngle<cubesx[i] && deltaAngle+0.001>=cubesx[i]-0.7 && cubesy[i]-deltaMove>-0.7 && cubesy[i]-deltaMove<0.7 && pd==4 && damage[i]!=3)
			px=0;
	if(deltaAngle>cubesx[i] && deltaAngle-0.001<=cubesx[i]+0.7 && cubesy[i]-deltaMove>-0.7 && cubesy[i]-deltaMove<0.7 && pd==3 && damage[i]!=3)
			px=0;
	if(deltaMove>cubesy[i] && deltaMove-0.001<=cubesy[i]+0.7 && deltaAngle-cubesx[i]>-0.7 && deltaAngle-cubesx[i]<0.7 && pd==2 && damage[i]!=3)
			py=0;
	if(deltaMove<cubesy[i] && deltaMove+0.001>=cubesy[i]-0.7 && deltaAngle-cubesx[i]>-0.7 && deltaAngle-cubesx[i]<0.7 && pd==1 && deltaMove-cubesy[i]>-0.7 && deltaMove-cubesy[i]<0.7 && damage[i]!=3)
			py=0;
	}
	
	glTranslatef(deltaAngle, deltaMove, 0.0f);
	// up
	if(pd==1){
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glRotatef(-120, 1.0f, 0.0f, 0.0f);
	}
	// left
	if(pd==3){
	glRotatef(-180, 0.0f, 1.0f, 0.0f);
	}
	// down
	if(pd==2){
	glRotatef(-90, 0.0f, 0.0f, 1.0f);
	glRotatef(20, 1.0f, 0.0f, 0.0f);
	}
	glutSolidSphere(0.25,10,10);
	

// Draw Eyes
	glPushMatrix();
	glColor3f(1.0f,1.0f,1.0f);
	glTranslatef(0.2f, 0.10f, 0.18f);
	glRotatef(90,0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

// Draw Nose
	glPushMatrix();
	glColor3f(1.0f, 0.5f , 0.5f);
	glTranslatef(0.24f, 0.0f, 0.0f);
	glRotatef(90,0.0f, 1.0f, 0.0f);
	glutSolidCone(0.07f,0.3f,10,2);
	glPopMatrix();
	glPopMatrix();
	// Draw feet
	/*
	glPushMatrix();
	glColor3f(1.0f,0.0f,0.0f);
	glTranslatef(0.2f, -0.2f, 0.18f);
	glRotatef(90,0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.1f,20,10);
	glTranslatef(-0.4f, -0.06f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();
	glPopMatrix();
	*/
	/*
    glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(px, py , 0.0f);
	glutSolidCube(0.7);
	glPopMatrix();
	*/
}

void drawGround(){
	glColor3f(0.5f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-ratio*3.1, -ratio*3.1, 0.0f );
		glVertex3f(-ratio*3.1, ratio*3.1, 0.0f);
		glVertex3f( ratio*3.1, ratio*3.1, 0.0f);
		glVertex3f( ratio*3.1, -ratio*3.1, 0.0f);
	glEnd();
}

void drawCubes(){
	int i,j,k;
	int rnum;
	float val=0.0;
	rnum = rand();
	k=0;
	glColor3f(1.0f, 1.0f, 1.0f);
	//if(initialized){


	for(i = 0; i < 16; i++){
	if(cubesx[i]<-3.65){
	cubesx[i]=-3.65;
	if(damagex==cubesx[i] && damagey==cubesy[i] && damage[i]<3)
		if(deltaAngle<cubesx[i] && deltaAngle+0.001>=cubesx[i]-0.7 && cubesy[i]-deltaMove>-0.3 && cubesy[i]-deltaMove<0.3 && pd==4 && damage[i]!=3 || deltaAngle>cubesx[i] && deltaAngle-0.001<=cubesx[i]+0.7 && cubesy[i]-deltaMove>-0.3 && cubesy[i]-deltaMove<0.3 && pd==3 && damage[i]!=3 || deltaMove>cubesy[i] && deltaMove-0.001<=cubesy[i]+0.7 && deltaAngle-cubesx[i]>-0.3 && deltaAngle-cubesx[i]<0.3 && pd==2 && damage[i]!=3 || deltaMove<cubesy[i] && deltaMove+0.001>=cubesy[i]-0.7 && deltaAngle-cubesx[i]>-0.3 && deltaAngle-cubesx[i]<0.3 && pd==1 && deltaMove-cubesy[i]>-0.7 && deltaMove-cubesy[i]<0.7 && damage[i]!=3)
		damage[i]+=1;
	cubesxm[i]=-3.65;
	}
	if(cubesx[i]>3.65){
	cubesx[i]=3.65;
	if(damagex==cubesx[i] && damagey==cubesy[i] && damage[i]<3)
		if(deltaAngle<cubesx[i] && deltaAngle+0.001>=cubesx[i]-0.7 && cubesy[i]-deltaMove>-0.3 && cubesy[i]-deltaMove<0.3 && pd==4 && damage[i]!=3 || deltaAngle>cubesx[i] && deltaAngle-0.001<=cubesx[i]+0.7 && cubesy[i]-deltaMove>-0.3 && cubesy[i]-deltaMove<0.3 && pd==3 && damage[i]!=3 || deltaMove>cubesy[i] && deltaMove-0.001<=cubesy[i]+0.7 && deltaAngle-cubesx[i]>-0.3 && deltaAngle-cubesx[i]<0.3 && pd==2 && damage[i]!=3 || deltaMove<cubesy[i] && deltaMove+0.001>=cubesy[i]-0.7 && deltaAngle-cubesx[i]>-0.3 && deltaAngle-cubesx[i]<0.3 && pd==1 && deltaMove-cubesy[i]>-0.7 && deltaMove-cubesy[i]<0.7 && damage[i]!=3)
		damage[i]+=1;
	    cubesxm[i]=3.65;
	}
	if(cubesy[i]<-3.65){
	cubesy[i]=-3.65;
	if(damagex==cubesx[i] && damagey==cubesy[i] && damage[i]<3)
		if(deltaAngle<cubesx[i] && deltaAngle+0.001>=cubesx[i]-0.7 && cubesy[i]-deltaMove>-0.3 && cubesy[i]-deltaMove<0.3 && pd==4 && damage[i]!=3 || deltaAngle>cubesx[i] && deltaAngle-0.001<=cubesx[i]+0.7 && cubesy[i]-deltaMove>-0.3 && cubesy[i]-deltaMove<0.3 && pd==3 && damage[i]!=3 || deltaMove>cubesy[i] && deltaMove-0.001<=cubesy[i]+0.7 && deltaAngle-cubesx[i]>-0.3 && deltaAngle-cubesx[i]<0.3 && pd==2 && damage[i]!=3 || deltaMove<cubesy[i] && deltaMove+0.001>=cubesy[i]-0.7 && deltaAngle-cubesx[i]>-0.3 && deltaAngle-cubesx[i]<0.3 && pd==1 && deltaMove-cubesy[i]>-0.7 && deltaMove-cubesy[i]<0.7 && damage[i]!=3)
		damage[i]+=1;
	cubesym[i]=-3.65;
	}
	if(cubesy[i]>3.65){
	cubesy[i]=3.65;
	if(damagex==cubesx[i] && damagey==cubesy[i] && damage[i]<3)
		if(deltaAngle<cubesx[i] && deltaAngle+0.001>=cubesx[i]-0.7 && cubesy[i]-deltaMove>-0.3 && cubesy[i]-deltaMove<0.3 && pd==4 && damage[i]!=3 || deltaAngle>cubesx[i] && deltaAngle-0.001<=cubesx[i]+0.7 && cubesy[i]-deltaMove>-0.3 && cubesy[i]-deltaMove<0.3 && pd==3 && damage[i]!=3 || deltaMove>cubesy[i] && deltaMove-0.001<=cubesy[i]+0.7 && deltaAngle-cubesx[i]>-0.3 && deltaAngle-cubesx[i]<0.3 && pd==2 && damage[i]!=3 || deltaMove<cubesy[i] && deltaMove+0.001>=cubesy[i]-0.7 && deltaAngle-cubesx[i]>-0.3 && deltaAngle-cubesx[i]<0.3 && pd==1 && deltaMove-cubesy[i]>-0.7 && deltaMove-cubesy[i]<0.7 && damage[i]!=3)
		damage[i]+=1;
	cubesym[i]=3.65;
	}
	}
	for(i = -2; i < 2; i++){
		for(j=-2; j < 2; j++) {
			rnum = rand();
			
			//val=rnum*ratio*i+1;
			if(j<0)
		    glColor3f(1.0f, 1.0f, 1.0f);
			glPushMatrix();
			val=(rand())%3;
			val+=0.1;
			while(val>=3.0 || val<=1){
			val=(rand())%3;
			val+=0.1;
			}
			if(ratio!=1.0f){
			int ii=0,safe=0;
				/*
				while(ii<k){
					if(rnum-cubesx[ii]>-0.7 && rnum-cubesx[ii]<0.7){
						rnum=rand(ctime())%3;
						ii=0;
					}
				ii++;
				}
				*/
				if(initialized==1){
					/*
					cubesx[k]=rnum;
					if(i<0)
					cubesx[k]= -cubesx[k];
					cubesy[k]=rnum;
					if(j<0)
					cubesy[k]= -cubesy[k];
					*/

					cubesx[k]=val*i+1+i/10;
				    cubesy[k]=val*j+1+j/10;

					//cubesx[k]=(ratio*i+1);
				    //cubesy[k]=ratio*j+1;
				}
				glTranslatef(cubesx[k], cubesy[k],0.0f);
			if(damage[k]==0)
				glColor3f(1.0f, 1.0f, 1.0f);
			if(damage[k]==1){
			glColor3f(0.0f, 0.0f, 0.0f);
			glRotatef(90, 0.0f, 0.0f, 1.0f);
			glBegin( GL_LINE_STRIP );
			glColor3f( 1.0, 0.0, 0.0 );
			glVertex3f( 0.36, 0.0, -0.35 );
			glVertex3f( 0.36, 0.0, 0.35 );
			glEnd();
			glRotatef(90, 0.0f, 0.0f, 1.0f);
			glBegin( GL_LINE_STRIP );
			glColor3f( 1.0, 0.0, 0.0 );
			glVertex3f( 0.36, 0.0, -0.35 );
			glVertex3f( 0.36, 0.0, 0.35 );
			glEnd();
			glRotatef(90, 0.0f, 0.0f, 1.0f);
			glBegin( GL_LINE_STRIP );
			glColor3f( 1.0, 0.0, 0.0 );
			glVertex3f( 0.36, 0.0, -0.35 );
			glVertex3f( 0.36, 0.0, 0.35 );
			glEnd();
			glRotatef(90, 0.0f, 0.0f, 1.0f);
			glBegin( GL_LINE_STRIP );
			glColor3f( 1.0, 0.0, 0.0 );
			glVertex3f( 0.36, 0.0, -0.35 );
			glVertex3f( 0.36, 0.0, 0.35 );
			glEnd();
			}
			if(damage[k]==2){
				//glColor3f(0.0f, 1.2f, 0.0f);
				glColor3f(0.0f, 0.0f, 0.0f);
			glRotatef(90, 0.0f, 0.0f, 1.0f);
			glBegin( GL_LINE_STRIP );
			glColor3f( 1.0, 0.0, 0.0 );
			glVertex3f( 0.36, 0.2, -0.35 );
			glVertex3f( 0.36, 0.2, 0.35 );
			glEnd();
			glBegin( GL_LINE_STRIP );
			glColor3f( 1.0, 0.0, 0.0 );
			glVertex3f( 0.36, -0.2, -0.35 );
			glVertex3f( 0.36, -0.2, 0.35 );
			glEnd();
			glRotatef(90, 0.0f, 0.0f, 1.0f);
			glBegin( GL_LINE_STRIP );
			glColor3f( 1.0, 0.0, 0.0 );
			glVertex3f( 0.36, 0.2, -0.35 );
			glVertex3f( 0.36, 0.2, 0.35 );
			glEnd();
			glBegin( GL_LINE_STRIP );
			glColor3f( 1.0, 0.0, 0.0 );
			glVertex3f( 0.36, -0.2, -0.35 );
			glVertex3f( 0.36, -0.2, 0.35 );
			glEnd();
			glRotatef(90, 0.0f, 0.0f, 1.0f);
			glBegin( GL_LINE_STRIP );
			glColor3f( 1.0, 0.0, 0.0 );
			glVertex3f( 0.36, 0.2, -0.35 );
			glVertex3f( 0.36, 0.2, 0.35 );
			glEnd();
			glBegin( GL_LINE_STRIP );
			glColor3f( 1.0, 0.0, 0.0 );
			glVertex3f( 0.36, -0.2, -0.35 );
			glVertex3f( 0.36, -0.2, 0.35 );
			glEnd();
			glRotatef(90, 0.0f, 0.0f, 1.0f);
			glBegin( GL_LINE_STRIP );
			glColor3f( 1.0, 0.0, 0.0 );
			glVertex3f( 0.36, 0.2, -0.35 );
			glVertex3f( 0.36, 0.2, 0.35 );
			glEnd();
			glBegin( GL_LINE_STRIP );
			glColor3f( 1.0, 0.0, 0.0 );
			glVertex3f( 0.36, -0.2, -0.35 );
			glVertex3f( 0.36, -0.2, 0.35 );
			glEnd();
			}
			if(damage[k]==3)
				glColor3f(1.0f, 0.0f, 0.0f);
			
			k++;
			} else {
			ratio = 1.4f;
			}

			if(damage[k-1]!=3){
			glColor3f(1.0f, 1.0f, 1.0f);
			glutSolidCube(0.7f);
			}
			glPopMatrix();
		}
	}
		for(i=15;i>=0;i--){
		for(j=15;j>=0;j--){

		//if(i!=j && cubesx[i]+0.001>=cubesx[j]-0.7 && (cubesy[i]<=cubesy[j]+0.3 && cubesy[i]>=cubesy[j]-0.3)){
		if(i!=j && cubesxm[i]!=0.0 && cubesxm[j]==0.0 && cubesx[i]<cubesx[j] && cubesx[i]+0.001>cubesx[j]-0.7 && cubesy[i]-cubesy[j]>-1.0 && cubesy[i]-cubesy[j]<1.0 && pd==4 && damage[i]!=3 && damage[j]!=3){
			cubesxm[i]=0;
		    if(damagex==cubesx[i] && damagey==cubesy[i] && damage[i]<3)
				damage[i]+=1;
		}
		else if(i!=j && cubesxm[i]!=0.0 && cubesxm[j]==0.0 && cubesx[i]>cubesx[j] && cubesx[i]-0.001<cubesx[j]+0.7  && cubesy[i]-cubesy[j]>-1.0 && cubesy[i]-cubesy[j]<1.0 && pd==3 && damage[i]!=3 && damage[j]!=3){
			cubesxm[i]=0;
			if(damagex==cubesx[i] && damagey==cubesy[i] && damage[i]<3)
				damage[i]+=1;
		}
		else if(i!=j && cubesym[i]!=0.0 && cubesym[j]==0.0 && cubesy[i]>cubesy[j] && cubesy[i]-0.001<cubesy[j]+0.7 && cubesx[i]-cubesx[j]>-1.0 && cubesx[i]-cubesx[j]<1.0 && pd==2  && damage[i]!=3 && damage[j]!=3){
			cubesym[i]=0;
			if(damagex==cubesx[i] && damagey==cubesy[i] && damage[i]<3)
				damage[i]+=1;
		}
		else if(i!=j && cubesym[i]!=0.0 && cubesym[j]==0.0 && cubesy[i]<cubesy[j] && cubesy[i]+0.001>cubesy[j]-0.7 && cubesx[i]-cubesx[j]>-1.0 && cubesx[i]-cubesx[j]<1.0 && pd==1 && damage[i]!=3 && damage[j]!=3){
			cubesym[i]=0;
			if(damagex==cubesx[i] && damagey==cubesy[i] && damage[i]<3)
				damage[i]+=1;
		}
		
		if(j==0)
		safeMove=1;
		}
		if(safeMove==1 && cubesxm[i]>0){
			pcx[i]=cubesxm[i];
			cubesxm[i]-=0.001;
			cubesx[i]+=0.001;
			safeMove=0;
			if(cubesxm[i]<=0)
			cubesxm[i]=0.0;
			//if(pcx[i]==cubesxm[i])
			//cubesxm[i]=0.0;
			}
		
		if(safeMove==1 && cubesxm[i]<0){
			pcx[i]=cubesxm[i];
			cubesxm[i]+=0.001;
			cubesx[i]-=0.001;
			safeMove=0;
			if(cubesxm[i]>=0)
			cubesxm[i]=0.0;
			if(pcx[i]==cubesxm[i])
			cubesxm[i]=0.0;
			}
		if(safeMove==1 && cubesym[i]<0){
			pcy[i]=cubesym[i];
			cubesym[i]+=0.001;
			cubesy[i]-=0.001;
			safeMove=0;
			if(cubesym[i]>=0)
			cubesym[i]=0.0;
			if(pcy[i]==cubesym[i])
			cubesym[i]=0.0;
			}
		if(safeMove==1 && cubesym[i]>0){
			pcy[i]=cubesym[i];
			cubesym[i]-=0.001;
			cubesy[i]+=0.001;
			safeMove=0;
			if(cubesym[i]<=0)
			cubesym[i]=0.0;
			if(pcy[i]==cubesym[i])
			cubesym[i]=0.0;
			}
		}
		
	initialized=0;
	//}
		
	
}

void renderScene(void) {

	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	if(status==1 || status==2){
		gluLookAt(	-7.0, -12.0f, 8.0, 4.0, 5.0f, -5.0, 0.0f, 1.0f,  12.0f);
	if(status==1){
	glColor3f(1,0,0);
	drawBitmapText("YOU WON!",0,0,0);
	}
	if(status==2){
	glColor3f(1,0,0);
	drawBitmapText("YOU LOSE!",0,0,0);
	}
	//glutSwapBuffers(); 
	
	} else {
	// Set the camera
	//gluLookAt(	10.0f, 0.0f, 15.0f,	-5.0f, 0.0f,  0.0f,	0.0f, 1.0f,  0.0f);
	//gluLookAt(10.0f, 0.0f, 15.0f, 2.0, 4.0, -3.0, 2.0, 2.0, -1.0 );
	//gluLookAt(	x, 1.0f, z,	x+lx, 1.0f,  z+lz, 0.0f, 1.0f,  0.0f);
	//gluLookAt(	0.0, -15.0f, 15.0, 0.0, -1.0f, -5.0, 0.0f, 5.0f,  0.0f);
	//gluLookAt(	-10.0, -15.0f, 8.0, 5.0, 5.0f, -5.0, 0.0f, 1.0f,  5.0f);
	gluLookAt(	-7.0, -12.0f, 8.0, 4.0, 5.0f, -5.0, 0.0f, 1.0f,  12.0f);
	//light();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	drawGround();
	glEnable(GL_DEPTH_TEST); 
 glEnable(GL_LIGHTING);
	light();
	drawCubes();
	light();
	drawPlayer();
	light();
	drawGhost();
	//glColor3f(1,0,0);
	//drawBitmapText("YOU WON!",0,0,0);
	//glRotatef(angle, 0.0f, 1.0f, 0.0f);

	/*
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(-ratio*3.4, 0.0f , 0.0f);
	glutSolidCube(1);
	glPopMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glutSolidCube(1);
	*/
	
	/*
	if(angle<=ratio*3.4){
	glTranslatef(angle, 0.0f, 0.0f);
	glutSolidCube(1);
	}else if (angle>=ratio*3.4){
	glTranslatef(ratio*3.4, 0.0f, 0.0f);
	glutSolidCube(1);
	}
	*/

	//glTranslatef(-angle, 0.0f, 0.0f);
	//glutSolidCube(2);
	//glPopMatrix();
	
	/*
	glBegin(GL_TRIANGLES);
		glVertex3f(-2.0f,-2.0f, 0.0f);
		glVertex3f( 2.0f, 0.0f, 0.0);
		glVertex3f( 0.0f, 2.0f, 0.0);
	glEnd();
	*/
	angle+=0.001f;
	//glFinish();
    
}
	glFinish();
	glutSwapBuffers();
}
void pressKey(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_LEFT : 
			px-=0.3;
			//if(deltaAngle>-3.65)
			//deltaAngle += -0.3f;
			if(booll==1){
			//deltaAngle += 0.3f;
			px=0;
			booll=0;
			}
			if(deltaAngle<-3.65)
			deltaAngle=-3.65;
			ppd = pd;
			pd = 3;
			break;
		case GLUT_KEY_RIGHT : 
			px+=0.3;
			//deltaAngle += 0.3f;
			if(boolr==1){
			//deltaAngle -= 0.3f;
			px=0;
			boolr=0;
			}
			if(deltaAngle>3.65)
			deltaAngle = 3.65;
			ppd = pd;
			pd = 4;
			break;
		case GLUT_KEY_UP : 
			py+=0.3;
			//deltaMove += 0.3f;
			if(boolu==1){
			//deltaMove -= 0.3f;
			py=0;
			boolu=0;
			}
			if(deltaMove>3.65)
			deltaMove=3.65;
			ppd = pd;
			pd = 1;
			break;
		case GLUT_KEY_DOWN : 
			py-=0.3;
			//deltaMove += -0.3f;
			if(boold==1){
			//deltaMove += 0.3f;
			py=0;
			boold=0;
			}
			if(deltaMove<-3.65)
			deltaMove=-3.65;
			ppd = pd;
			pd = 2;
			break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT : break;
		case GLUT_KEY_RIGHT : break;
		case GLUT_KEY_UP : break;
		case GLUT_KEY_DOWN : break;
	}
}

void processNormalKeys(unsigned char key, int x, int y) {
	int i=0,j=0;
	float t0,t1,t2,t3;
	if(key == 27)
		exit(0);
	if (key == 13){
	    for(i=15;i>=0;i--){
			t2=cubesx[i];
			t3=cubesy[i];
			t0=cubesx[i]-deltaAngle;
			t1=cubesy[i]-deltaMove;
		//if(deltaAngle>=cubesx[i]-0.8 && (deltaMove<=cubesy[i]+0.3 && deltaMove>=cubesy[i]-0.3) && pd==4){
		//if(t0>-1.0 && t0<1.0 && t1>-1.0 && t1<1.0 && pd==4){
			/*
		if(cubesx[i]>=3.65 && pd==4){
			damagex=cubesx[i];
			damagey=cubesy[i];
			break;
		}
		if(cubesx[i]<=-3.65 && pd==3){
			damagex=cubesx[i];
			damagey=cubesy[i];
			break;
		}
		if(cubesy[i]<=-3.65 && pd==2){
			damagex=cubesx[i];
			damagey=cubesy[i];
			break;
		}
		if(cubesy[i]>=3.65 && pd==1){
			damagex=cubesx[i];
			damagey=cubesy[i];
			break;
		}
		*/
		if(deltaAngle<cubesx[i] && deltaAngle+0.001>=cubesx[i]-0.7 && cubesy[i]-deltaMove>-0.3 && cubesy[i]-deltaMove<0.3 && pd==4 && damage[i]!=3 || deltaAngle>cubesx[i] && deltaAngle-0.001<=cubesx[i]+0.7 && cubesy[i]-deltaMove>-0.3 && cubesy[i]-deltaMove<0.3 && pd==3 && damage[i]!=3 || deltaMove>cubesy[i] && deltaMove-0.001<=cubesy[i]+0.7 && deltaAngle-cubesx[i]>-0.3 && deltaAngle-cubesx[i]<0.3 && pd==2 && damage[i]!=3 || deltaMove<cubesy[i] && deltaMove+0.001>=cubesy[i]-0.7 && deltaAngle-cubesx[i]>-0.3 && deltaAngle-cubesx[i]<0.3 && pd==1 && deltaMove-cubesy[i]>-0.7 && deltaMove-cubesy[i]<0.7 && damage[i]!=3)
		if(deltaAngle<cubesx[i] && cubesx[i]-deltaAngle>-1.0 && cubesx[i]-deltaAngle<1.0 && cubesy[i]-deltaMove>-1.0 && cubesy[i]-deltaMove<1.0 && pd==4 && damage[i]!=3){
			cubesxm[i]=2.0;
			damagex=cubesx[i];
			damagey=cubesy[i];
			break;
		}
		
		if(deltaAngle<cubesx[i] && deltaAngle+0.001>=cubesx[i]-0.7 && cubesy[i]-deltaMove>-0.3 && cubesy[i]-deltaMove<0.3 && pd==4 && damage[i]!=3 || deltaAngle>cubesx[i] && deltaAngle-0.001<=cubesx[i]+0.7 && cubesy[i]-deltaMove>-0.3 && cubesy[i]-deltaMove<0.3 && pd==3 && damage[i]!=3 || deltaMove>cubesy[i] && deltaMove-0.001<=cubesy[i]+0.7 && deltaAngle-cubesx[i]>-0.3 && deltaAngle-cubesx[i]<0.3 && pd==2 && damage[i]!=3 || deltaMove<cubesy[i] && deltaMove+0.001>=cubesy[i]-0.7 && deltaAngle-cubesx[i]>-0.3 && deltaAngle-cubesx[i]<0.3 && pd==1 && deltaMove-cubesy[i]>-0.7 && deltaMove-cubesy[i]<0.7 && damage[i]!=3)
		if(deltaAngle>cubesx[i] && cubesx[i]-deltaAngle>-1.0 && cubesx[i]-deltaAngle<1.0 && cubesy[i]-deltaMove>-1.0 && cubesy[i]-deltaMove<1.0 && pd==3 && damage[i]!=3){
			cubesxm[i]=-2.0;
			damagex=cubesx[i];
			damagey=cubesy[i];
			break;
		}
		if(deltaAngle<cubesx[i] && deltaAngle+0.001>=cubesx[i]-0.7 && cubesy[i]-deltaMove>-0.3 && cubesy[i]-deltaMove<0.3 && pd==4 && damage[i]!=3 || deltaAngle>cubesx[i] && deltaAngle-0.001<=cubesx[i]+0.7 && cubesy[i]-deltaMove>-0.3 && cubesy[i]-deltaMove<0.3 && pd==3 && damage[i]!=3 || deltaMove>cubesy[i] && deltaMove-0.001<=cubesy[i]+0.7 && deltaAngle-cubesx[i]>-0.3 && deltaAngle-cubesx[i]<0.3 && pd==2 && damage[i]!=3 || deltaMove<cubesy[i] && deltaMove+0.001>=cubesy[i]-0.7 && deltaAngle-cubesx[i]>-0.3 && deltaAngle-cubesx[i]<0.3 && pd==1 && deltaMove-cubesy[i]>-0.7 && deltaMove-cubesy[i]<0.7 && damage[i]!=3)
		if(deltaMove>cubesy[i] && cubesy[i]-deltaMove>-1.0 && cubesy[i]-deltaMove<1.0 && cubesx[i]-deltaAngle>-1.0 && cubesx[i]-deltaAngle<1.0 && pd==2 && damage[i]!=3){
			cubesym[i]=-2.0;
			damagex=cubesx[i];
			damagey=cubesy[i];
			break;
		}
		if(deltaAngle<cubesx[i] && deltaAngle+0.001>=cubesx[i]-0.7 && cubesy[i]-deltaMove>-0.3 && cubesy[i]-deltaMove<0.3 && pd==4 && damage[i]!=3 || deltaAngle>cubesx[i] && deltaAngle-0.001<=cubesx[i]+0.7 && cubesy[i]-deltaMove>-0.3 && cubesy[i]-deltaMove<0.3 && pd==3 && damage[i]!=3 || deltaMove>cubesy[i] && deltaMove-0.001<=cubesy[i]+0.7 && deltaAngle-cubesx[i]>-0.3 && deltaAngle-cubesx[i]<0.3 && pd==2 && damage[i]!=3 || deltaMove<cubesy[i] && deltaMove+0.001>=cubesy[i]-0.7 && deltaAngle-cubesx[i]>-0.3 && deltaAngle-cubesx[i]<0.3 && pd==1 && deltaMove-cubesy[i]>-0.7 && deltaMove-cubesy[i]<0.7 && damage[i]!=3)
		if(deltaAngle<cubesx[i] && cubesy[i]-deltaMove>-1.0 && cubesy[i]-deltaMove<1.0 && cubesx[i]-deltaAngle>-1.0 && cubesx[i]-deltaAngle<1.0 && pd==1 && damage[i]!=3){
			cubesym[i]=2.0;
			damagex=cubesx[i];
			damagey=cubesy[i];
			break;
		}
	}
	}
}

void onInitialization( ) { //creating the light source
 glEnable(GL_DEPTH_TEST); 
 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);
 glEnable(GL_COLOR_MATERIAL);
 GLfloat diffuse[]={1.0, 1.0, 1.0, 1.0};
 //GLfloat pos[]={0.2, 8.0, 8.0, 1.0};
 GLfloat pos[]={1.0, -1.0f, 1.0, 1.0};

     glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);

   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redDiffuseMaterial);

 glLightfv(GL_LIGHT0, GL_POSITION, pos);
 //glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
 
 //glDisable(GL_LIGHTING);
 //glDisable(GL_DEPTH_TEST); 
 //glDisable(GL_LIGHT0);
 //glRotatef(-90, 1, 0, 0); //we want to see the top of the teapot
}

int main(int argc, char **argv) {
	srand(clock());
	ghostMove[0]=1.0;
	ghostAngle[0]=1.0;
	ghostMove[1]=2.0;
	ghostAngle[1]=2.0;
	ghostMove[2]=-1.0;
	ghostAngle[2]=-1.0;
	ghostMove[3]=-2.0;
	ghostAngle[3]=-2.0;
	gx[0]=100;
	gy[0]=100;
	gx[1]=200;
	gy[1]=200;
	gx[2]=300;
	gy[2]=300;
	gx[3]=400;
	gy[3]=400;

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(250,100);
	glutInitWindowSize(600,600);
	glutCreateWindow("SnowMan Game By Abdulellah Alsaheel");

	onInitialization();
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutSpecialFunc(pressKey);
	glutKeyboardFunc(processNormalKeys);
	// here are the new entries
	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(releaseKey);
	// enter GLUT event processing cycle
	
	glutMainLoop();

	return 0;
}