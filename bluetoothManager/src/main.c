/*
 *  
 *  Bluethooth  scanner  based on bluez  [[  bluetooth protocol @linux  stack ]]  
 *  Copyright  (c) 2022 Umar <jukoo> [jUmarB@protonmail.com]   
 *
 */


#include  <stdio.h> 
#include  <stdlib.h> 
#include  <unistd.h>
#include  <sys/socket.h> 
#include  <bluetooth/bluetooth.h>  
#include  <bluetooth/hci.h> 
#include  <bluetooth/hci_lib.h> 
#include  <assert.h>  
#include  <errno.h>  
#include  <string.h>  

#define  MAX_NRSP  0xff  
#define  MAX_LEN   0x8 
#define  BDADDR_MAX_BUFF 0x14  
#define  INVALIDADDR  "00:00:00:00:00:00"

int 
main  ( int argc , char **argv )  { 

    
   inquiry_info  *query_bluetooth_info  =   ( void * ) 0 ;  
   
   int device_identifier  = hci_get_route((void *) 0 ) ; 
   int device_socket      = hci_open_dev(device_identifier) ;    
   assert ( device_identifier >=0  &&  device_socket >0 ) ; 

   char device_name[MAX_NRSP]  = { 0  } ;  
   char device_addr[248]={0} ;  

   query_bluetooth_info  = (inquiry_info*)  malloc(MAX_NRSP *sizeof(inquiry_info)) ;  
   assert(query_bluetooth_info !=  (void *) 0 ) ;  
 
   if  ( hci_inquiry(device_identifier,  MAX_LEN ,MAX_NRSP ,(void *) 0 , &query_bluetooth_info ,IREQ_CACHE_FLUSH ) < 0 )  
   {
   
        perror ("query info : " ) ; 
        exit(EXIT_FAILURE)  ; 
   }  
   

   for ( int i = 0  ;  i  <  MAX_NRSP  ; i++ )  
   {
       ba2str( &(query_bluetooth_info+i)->bdaddr , device_addr); 
       memset (device_name ,0 ,   sizeof(device_name)) ; 
       if ( hci_read_remote_name(device_socket , &(query_bluetooth_info+i)->bdaddr ,  sizeof(device_name)  , device_name , 0 ) < 0 ) 
       {
        
           memset (device_name ,0 ,   sizeof(device_name)) ; 
       }  
       if(strcmp(device_addr , INVALIDADDR) != 0 ) 
       {
           fprintf (stdout , "%s  ::  %s \n" ,device_addr , device_name)  ;  
       }
       
   }
  
    
   free(query_bluetooth_info)  ;   
   close(device_socket) ;  
   return EXIT_SUCCESS ; 
} 
