/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package askisi4;

import java.util.Random;


//public enum Movement{ impressionism, expressionism, naturalism };
//public enum Condition{ bad, good, excellent };

//public enum Technique{ oil, aquarelle, tempera };
//public enum Material{ iron, stone, wood };



/**
 *

 */


public class Askisi4 {

    /**
     * @param args the command line arguments
     */

    
	public static void main(String[] args) {
       
	
    //srand(time(null));
	
    if( args.length < 3){
	
	System.out.print("Not enough arguments\n");
	
        return;
    }	
															
		
    int N = Integer.parseInt(args[0]);
	//System.out.print("N is "+N+"\n");
		
    Movement mo = Movement.valueOf(args[1]);
	//int m = atoi(argv[2]);
	//cout << "M is " << mo <<endl;
		
    Condition co = Condition.valueOf(args[2]);
	//cout << "c is "<< co <<endl;
	
    Artifacts[] a = new Artifacts[N]; 		//allocating memory
	
    int j=0;
	
    StringBuilder s1= new StringBuilder();
    String cr1;
	
    Random r = new Random();
		
	for(int i=0;i<N;i++){				//initialising randomly
		
		
            //Movement m1 = (Movement)(rand() % 3));//impressionism,expressionism,naturalism
            Movement m1 = null;
            int pick_m = r.nextInt(3);
            
            switch(pick_m){
                case 0 :m1 = Movement.impressionism;
                        break;
                        
                case 1 :m1 = Movement.expressionism;         
                        break;
                        
                case 2: m1 = Movement.naturalism;        
                        break;
                default: System.out.print("ERROR 1\n");
                        break;        
            }
            
            
            
            //Condition c1 = (Condition)(rand() % 3);    // bad,good,excellent
            Condition c1 = null;
            int pick_c = r.nextInt(3);
            
            switch(pick_c){
                case 0 :c1 = Condition.bad;
                        break;
                        
                case 1 :c1 = Condition.good;         
                        break;
                        
                case 2: c1 = Condition.excellent;        
                        break;
                default: System.out.print("ERROR 2\n");
                        break;        
            }
            
            
		
            s1.append("Creator");									
            s1.append(i);                         // CreatorI , I is integer                                
                
            cr1 = s1.toString();
            //Random r = new Random();
            
            int year = r.nextInt(2018);     //year = [0,2017]
            
            //int year = rand() % 2018;	// year = [0,2017] 
		
		

		
            j = r.nextInt(2);		//j=0(paintings) or j=1(sculpture)
		
            if(j==0){		//paintings
			
		float l = r.nextFloat();                //length
                
		float w = r.nextFloat();                //width
                
                //Technique T = (Technique)(rand() % 3);					//oil,aquarelle,tempera
                
                Technique T = null;
                int pick_T= r.nextInt(3);
                
                switch(pick_T){
                case 0 :T = Technique.oil;
                        break;
                        
                case 1 :T = Technique.aquarelle;         
                        break;
                        
                case 2: T = Technique.tempera;        
                        break;
                default: System.out.print("ERROR 2\n");
                        break;        
                }
                
			
		a[i] = new Paintings(l,w,T,m1,c1,i,cr1,year);	
		}
		if(j==1){		//sculpture
		
			//float vol = ( (float)rand() ) / ( (float)(RAND_MAX/32.0) );			//volume
                    float vol = r.nextFloat();
                        
                    Material mat = null;
                    int pick_m1 =r.nextInt(3);
                    
                    switch(pick_m1){
                        case 0 :mat = Material.iron;
                                break;
                        
                        case 1 :mat = Material.stone;         
                                break;
                        
                        case 2: mat = Material.wood;        
                                break;
                        default: System.out.print("ERROR 2\n");
                                break;        
                    }
                    
			//Material mat = (Material)(rand() % 3);					//iron,stone wood
			

                        //while(mat > 3 || mat < 0) Material mat = (Material)(rand() % 3); //in case we dont get desired 0,1,2
			
                    a[i] = new Sculptures(vol,mat,m1,c1,i,cr1,year);
			
		}
		
                s1.delete(0,9);
                
                
		//s1.str( std::string() );			//clearing the string stream
		//s1.clear();
		
		
	}		//initialisation of array complete!
	
	
System.out.print("\n\n		Starting auction	\n\n");	
	
GLOBAL.auction(a, mo, co);	        
        






    }
    
}







