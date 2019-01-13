/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package askisi4;

/**
 *

 */

//enum Movement{ impressionism, expressionism, naturalism };
//enum Condition{ bad, good, excellent };

//enum Technique{ oil, aquarelle, tempera };
//enum Material{ iron, stone, wood };



public abstract class Masterpiece extends Artifacts {
    	
//public:
	//public enum Movement{ impressionism, expressionism, naturalism  };
	//public enum Condition{ bad, good, excellent }; 
	
        private final Movement movement;
	private Condition condition;
	
		
Masterpiece(Movement m,Condition c,int i1,String s1,int y1)
{
    super(i1,s1,y1);
            
    movement = m;
    condition = c;        
            
    System.out.println("Creating an instance of Masterpiece... \n\n");
}
	
public void setCondition(Condition C){ condition = C;}
	
Condition getCondition(){ return condition;}
Movement getMovement(){ return movement;}
	
	
public @Override void getInfo(){
		
    //Artifacts::getInfo();
    super.getInfo();
    if((movement == Movement.impressionism ) && (condition == Condition.bad)){
        System.out.print("Movement: Impressionism  Condition: Bad \n");
    }
    if((movement == Movement.impressionism ) && (condition==Condition.good)){    
        System.out.print("Movement: Impressionism  Condition: Good \n");
    }
    
    
    if((movement == Movement.impressionism) && (condition==Condition.excellent)){
       System.out.println("Movement: Impressionism  Condition: Excellent \n");
    }

    
    
    
    
    
    if(movement == Movement.expressionism && condition==Condition.bad){
        
        System.out.print("Movement: Expressionism  Condition: Bad \n");
    }
        
    if(movement == Movement.expressionism && condition==Condition.good){
        
        System.out.print("Movement: Expressionism  Condition: Good \n");
    }
    if(movement == Movement.expressionism && condition==Condition.excellent){
        
        System.out.print("Movement: Expressionism  Condition: Excellent \n");
    }
    
    
    
    
    
    
    
    if(movement == Movement.naturalism && condition==Condition.bad){
       
        System.out.print("Movement: Naturalism  Condition: Bad \n");
    }
    if(movement == Movement.naturalism && condition==Condition.good){
        
       System.out.print("Movement: Naturalism  Condition: Good \n");
    }
    
    
    if(movement == Movement.naturalism && condition== Condition.excellent){
        
        System.out.print("Movement: Naturalism  Condition: Excellent \n");
    }			
}

//virtual bool evaluate(Movement &m,Condition c) = 0; 		//pure virtual

//bool evaluate(Movement &m,Condition c){};
	
};
    
    
    
    

