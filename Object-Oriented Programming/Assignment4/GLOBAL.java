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
public class GLOBAL {
    
 public static void auction(Artifacts[] arr,Movement m,Condition c){
	
	//System.out.print("N is : "+N);
	
	for(int i=0;i<arr.length ;i++){
		
            arr[i].getIndex();
            arr[i].getInfo();
		
            System.out.print("\n\n\n 		Time to evaluate: ");
		
		
            if(arr[i].evaluate(m,c) == true){
                
                System.out.print("Artifact accepted \n\n\n");    
            }    
            else{
		
		System.out.print("Artifact not accepted \n\n\n");
		
		}
	}
	
	
	
}
    
    
    
}
