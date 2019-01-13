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
public class Sculptures extends Masterpiece {
    
    //public enum Material{ iron, stone, wood };

//private: 
	private final float volume;	
	private final Material material;
	
//public:

Sculptures(float v,Material m1,Movement m,Condition c,int i,String s,int y)
{
    super(m,c,i,s,y);
    
    volume = v;
    material = m1; 
   
    System.out.print("Creating an instance of Sculptures... \n\n");
}		
		
public @Override void getInfo(){
		
    System.out.print("Sculpture\n");
		
    //Masterpiece::getInfo();
    super.getInfo();
    
    if(material == Material.iron) {
	System.out.print("Volume: "+ volume +" Material: iron \n");
	return;
    }
		
    if(material == Material.stone){
	System.out.print("Volume: "+ volume +" Material: stone \n"); 
	return;
    }
		
    if(material == Material.wood){
		
	System.out.print("Volume: "+ volume +" Material: wood \n");     
    } 
		
		
}


public void evaluate(Movement m){
    
    this.evaluate(m,Condition.excellent);
    
}

	
public @Override boolean  evaluate(Movement m,Condition c){
		
if( (super.getMovement()== m) && (super.getCondition() == c) ) return true;
		
return false;	
}	
    
    
    
    
}
