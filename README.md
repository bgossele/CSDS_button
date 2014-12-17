CSDS_button
===========

Once the components are deployed and initialised, all you have to do is wire them together like this:    

wireLocal 512 10 11 <nodeAddress>  
wireLocal 513 11 10 <nodeAddress>    

And wire event 413 from component 10 on the node to your backend component.  

More info on the LooCI middleware framework can be found [here](https://distrinet.cs.kuleuven.be/software/looci/).
