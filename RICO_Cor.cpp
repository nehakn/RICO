#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<conio.h>
#include<math.h>

float RICO_Dct(float, int);
float RICO_Test(float);

int main()
{
	char buffer[1024];
	char *record, *line;
	int file_row=0, file_col;
	int entries = 100;
	float mat[entries][3];
	FILE *fstream = fopen("DHT11Moisture1.csv", "r");
	printf("\nSensor data:\nTemp\tHumid\tMois\n");
	
	if(fstream == NULL)
	{
		printf("\nDHT file opening failed\n");
		return -1;
	}
	
	while ((line=fgets(buffer, sizeof(buffer), fstream)) != NULL && file_row<entries)
	{
		file_col=0;
		record = strtok(line, ",");
		while (record != NULL)
		{
			//printf("i: %d\t", i);
			printf("1: %s\n", record);
			mat[file_row][file_col] = atoi(record);
			printf("%d\t", mat[file_row][file_col]);
			record = strtok(NULL, ",");
			file_col++;
		}
		//printf("\n");
		file_row++;
	}

	printf ("\nData Size: %d\nData Type: %d\n", file_row, entries);

	int data_col;
	for (data_col=0; data_col<3; data_col++)
	{
		// reshape
		int num_iter = floor(file_row/(8*8));
		int iter_count, loc_row, loc_col;
		float f1_block[8][8];
		printf("%d\n", num_iter);
		for (iter_count=0; iter_count<num_iter; iter_count++)
		{
			for (loc_row=0; loc_row<8; loc_row++)
			{
				for (loc_col=0; loc_col<8; loc_col++)
				{
					f1_block[loc_row][loc_col] = mat[iter_count*64+loc_row*8+loc_col][data_col];
					printf("%f\t", f1_block[loc_row][loc_col]);
				}
				//printf("\n");
			}

			float pi = 22/7;
			int ROWs, COLs, i, j;
			int row =  sizeof f1_block / sizeof f1_block[0];
			int col = sizeof f1_block[0] / sizeof(int);
			float f_image[8][8], z[8][8], z1[8][8], x[8], idct_int[8][8], idct_mtx[8][8];
			float outDct[row][col], outApp[row][col];
			int c1, d1, e1, sum = 0;
			float dct_mtx[8][8] = {{0.3536,    0.3536,   0.3536,    0.3536,    0.3536,    0.3536,    0.3536,    0.3536},
		    {0.4904,    0.4157,    0.2778,    0.0975,   -0.0975,   -0.2778,   -0.4157,   -0.4904},
		    {0.4619,    0.1913,   -0.1913,   -0.4619,   -0.4619,   -0.1913,    0.1913,    0.4619},
		    {0.4157,   -0.0975,   -0.4904,   -0.2778,    0.2778,    0.4904,    0.0975,   -0.4157},
		    {0.3536,   -0.3536,   -0.3536,    0.3536,    0.3536,   -0.3536,   -0.3536,    0.3536},
		    {0.2778,   -0.4904,    0.0975,    0.4157,   -0.4157,   -0.0975,    0.4904,   -0.2778},
		    {0.1913,   -0.4619,    0.4619,   -0.1913,   -0.1913,    0.4619,   -0.4619,    0.1913},
		    {0.0975,   -0.2778,    0.4157,   -0.4904,    0.4904,   -0.4157,    0.2778,   -0.0975}};
			float a, b, c, d, e, f, g;
			ROWs=row/8;
			COLs=col/8;
			int i1, j1, k, k1;
			printf("hello1");
			a = RICO_Test(pi/16);
			b = RICO_Test(2*pi/16);
			d = RICO_Test(4*pi/16);
			c = (a+d)/2;
			e = (a+b)/4;
			f = a/2;
			g = b/4;
			printf("Row: %d\tCol: %d", row, col);

			for (i=0;i<8;i++)
			{
				for (j=0;j<8;j++)
				{
					z[i][j] = 0;
					z1[i][j] = 0;
				}
			}
			printf("The values of DCT coefficients are:\na = %f\nb = %f\nc = %f\nd = %f\ne = %f\nf = %f\ng = %f ", a, b, c, d, e, f ,g);
			for (i=0;i<ROWs;i++)
			{
				for (j=0;j<COLs;j++)
				{
					// break into 8x8 chunks
					for (i1=0;i1<8;i1++)
					{
						for (j1=0;j1<8;j1++)
						{
							f_image[i1][j1] = f1_block[i*8+i1][j*8+j1];
							//printf("%d\t", i1);
						}
						//printf("\n");
					}

					// dct

					for (k=0;k<8;k++)
					{
						for (k1=0;k1<8;k1++)
						{
							x[k1] = f_image[k][k1];
						}
						z[0][k] = d*(x[0]+x[7] + x[1]+x[6] + x[2]+x[5] + x[3]+x[4]);
						z[1][k] = a*(x[0]-x[7]) + c*(x[1]-x[6]) + e*(x[2]-x[5]);
						z[2][k] = b*(x[0]+x[7]) + f*(x[1]+x[6]) - f*(x[2]+x[5])-b*(x[3]+x[4]);
						z[3][k] = c*(x[0]-x[7]) - g*(x[1]-x[6]) - a*(x[2]-x[5]);
					}

					for (k=0;k<8;k++)
					{
						for (k1=0;k1<8;k1++)
						{
							x[k1] = z[k][k1];
						}
						z1[0][k] = d*(x[0]+x[7] + x[1]+x[6] + x[2]+x[5] + x[3]+x[4]);
						z1[1][k] = a*(x[0]-x[7]) + c*(x[1]-x[6]) + e*(x[2]-x[5]);
						z1[2][k] = b*(x[0]+x[7]) + f*(x[1]+x[6]) - f*(x[2]+x[5])-b*(x[3]+x[4]);
						z1[3][k] = c*(x[0]-x[7]) - g*(x[1]-x[6]) - a*(x[2]-x[5]);
					}


					//idct
					for (c1 = 0; c1 < 8; c1++)
					{
						for (d1 = 0; d1 < 8; d1++)
						{
					        for (e1 = 0; e1 < 8; e1++)
							{
					          sum = sum + dct_mtx[e1][c1]*z1[e1][d1];
		        			}
		       			idct_int[c1][d1] = sum;
		        		sum = 0;
		      			}
		    		}

		    		for (c1 = 0; c1 < 8; c1++)
					{
						for (d1 = 0; d1 < 8; d1++)
						{
					        for (e1 = 0; e1 < 8; e1++)
							{
					          sum = sum + idct_int[c1][e1]*dct_mtx[e1][d1];
		        			}
		       			idct_mtx[c1][d1] = sum;
		        		sum = 0;
		      			}
		    		}

					for (i1=0; i1<8; i1++)
					{
						for (j1=0; j1<8; j1++)
						{
							outDct[i*8+i1][j*8+j1] = round(z1[i1][j1]);
							outApp[i*8+i1][j*8+j1] = round(idct_mtx[i1][j1]);
						}
					}

				}
			}

		}
	}

}

float RICO_Test (float angle_in_rad)
{
	printf("hello test");
	float val, pi = 22/7;
	if (angle_in_rad == pi/16)
	{
		val = RICO_Dct(angle_in_rad, 1);
		val = val + val/32;
		printf("hello a");
	}
	//printf("Coefficient a is: %f\n", a);
	else if (angle_in_rad == 2*pi/16)
	{
		val = RICO_Dct(angle_in_rad, 2);
	}
	//printf("Coefficient b is: %f\n", b);
	else if (angle_in_rad == 4*pi/16)
	{
		val = RICO_Dct(angle_in_rad, 3);
	}
	//printf("Coefficient d is: %f\n", d);
	printf("%f\n", val);
	return val;
	
}

// rico for cosine and sine computation

float RICO_Dct(float theta_input, int sel)
{
	printf("hello dct");
	float pi=22/7;
	//int i=0, j=1, k=2, l=3, m=3, n=1;
	int o, p, q, r; 
	float  d1, sf_extra1, sf_extra2, ele_11, ele_21;
	float d[2][1], v[2][1];
	theta_input = theta_input*180/pi;
	if(theta_input>=0 && theta_input<=10)
	{
		o = -1;
		p = -1;
		q = -1;
		r = -1;
		sf_extra1 = 0.6041;
		sf_extra2 = 0.6041;
	}
	else if(theta_input>=1 && theta_input<=15)
	{
		o = -1;
		p = -1;
		q = 1;
		r = 1;
		sf_extra1 = 0.6041;
		sf_extra2 = 0.4220;
	}
	else if(theta_input >= 16 && theta_input <= 20)
	{
	    o = -1;
	    p = -1;
	    q = 1;
	    r = 1;
	    sf_extra1 = 0.6041;
	    sf_extra2 = 0.6041;
	}
    
 	else if(theta_input >=21 && theta_input <= 25)
 	{
 		o = -1;
	    p = 1;
	    q = -1;
	    r = -1;
	    sf_extra1 = 0.6041;
	    sf_extra2 = 0.6041;
	}
   
    
    else if(theta_input >=26 && theta_input <= 30)
    {
    	o = -1;
	    p = 1;
	    q = -1	;
	    r = -1;
	    sf_extra1 = 0.54;
	    sf_extra2 = 0.96;
	}
    

	else if (theta_input >=31 && theta_input <= 35)
	{
		o = -1;
	    p = 1;
	    q = 0;
	    r = 0;
	    sf_extra1 = 0.6041;
	    sf_extra2 = 0.6041;
	} 
    
    else if(theta_input >=36 && theta_input <=40)
    {
    	o = -1;
	    p = 1;
	    q = 0;
	    r = 0;
	    sf_extra1 = 0.6041;
	    sf_extra2 = 0.724;
	}
     

	else if(theta_input >=41 && theta_input <= 50)
	{
	    o = 1;
	    p = -1;
	    q = -1;
	    r = -1;
	    sf_extra1 = 0.6041;
	    sf_extra2 = 0.6041;
	}
    
    else if  (theta_input >=51 && theta_input <= 60)
	{
	    o = -1;
	    p = 1;
	    q = 1;
	    r = 1;
	    sf_extra1 = 0.483;
	    sf_extra2 = 0.664;
	}
    
    else if (theta_input >=61 && theta_input <= 70)
	{
	    o = 1;
	    p = -1;
	    q = 1;
	    r = 1;
	    sf_extra1 = 0.785;
	    sf_extra2 = 0.6041;
	}
    
    else if (theta_input >=71 && theta_input <= 80)
	{
	    o = 1;
	    p = -1;
	    q = 1;
	    r = 1;
	    sf_extra1 = 0.6041;
	    sf_extra2 = 0.6041;
	}
    
    else if(theta_input >=81 && theta_input <=90)
	{
	    o = 1;
	    p = 1;
	    q = -1;
	    r = -1;
	    sf_extra1 = 0.302;
	    sf_extra2 = 0.6041;
	}
	
	ele_11 = 1 - o*0.5 - p*0.25 - o*p*0.125;
	ele_21 = 1 + o*0.5 + p*0.25 - o*p*0.125;
	
	v[1][1] = ele_11*0.9844 - ele_21*r*0.25;
	v[2][1] = ele_11*r*0.25 + ele_21*0.9844;
	
	v[1][1] = v[1][1]*sf_extra1;
	v[2][1] = v[2][1]*sf_extra2;
	d[1][1] = 0.5*v[1][1];
	d[2][1] = 0.5*v[2][1];
	printf("bye dct\n");
	if (sel==3)
	{
		d1 = (d[1][1] + d[2][1])*0.5;
		return d1;	
	}
	else
		printf("%f\n", d[1][1]);
		return d[1][1];
}
