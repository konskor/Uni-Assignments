/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package askisi4;

//public enum Movement { impressionism,expressionism,naturalism};
//public enum Condition { bad, good, excellent };

//public enum Technique { oil, aquarelle, tempera}
//public enum Material { iron, stone, wood }





public abstract class Artifacts {
    
    private final int index;
    private final String creator;
    private final int year; 
   
    
Artifacts(int i,String s,int y){
	
    index = i;
    creator = s;
    year = y;
    
    
   System.out.println("Creating an instance of Artifacts... \n\n");		
}
	


public void getInfo(){ 
    System.out.println("Creator: "+creator+"  Year: "+year+"\n");
}
public void getIndex(){ System.out.println("Index: "+index+"\n");}
		
public abstract boolean evaluate( Movement m, Condition c); 
    
}
