/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package askisi4;

/**
 *
 * @author kostantinos
 */
public class Paintings extends Masterpiece{
    
 //public enum Technique{ oil, aquarelle, tempera };

//private:
	private final float length;
	private final float width;
	private final Technique technique;
	
//public:
	
Paintings(float l,float w,Technique t,Movement m,Condition c,
                                                           int i,String s,int y)
	{
            super(m,c,i,s,y);
            
            length = l;
            width = w;
            technique = t;
            
            System.out.print("Creating an instance of Paintings... \n\n");		
	}
			
public @Override void getInfo(){
		
    System.out.print("Painting \n");
    
    //Masterpiece::getInfo();
    super.getInfo();
		
    float surface = width * length;
		
    if(technique == Technique.oil){
        
        System.out.print("Technique: Oil  Surface: "+surface+"\n");
    }
    
    
    if(technique == Technique.aquarelle){
    
        System.out.print("Technique: Aquarelle  Surface: "+surface+"\n");
    }
    
    if(technique == Technique.tempera){
        
        System.out.print("Technique: Tempera  Surface: "+surface+"\n");
    }
}

public void evaluate(Movement m){       //for default value
    
    this.evaluate(m,Condition.good);
}


	
public @Override boolean evaluate(Movement m,Condition c){
		
//if(Masterpiece::getMovement() == m && Masterpiece::getCondition() >= c) return true;
    if(super.getMovement() == m ){
        if((super.getCondition() == c) || (super.getCondition() == Condition.excellent)){
            return true;
        }
	
    return false;
    }
	   
 return false;      
}

}
