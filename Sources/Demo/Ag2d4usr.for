C Demo fuer Bar-Charts User Manual A8 / Ergaenzung zu AG2DEMO4.FOR
C
C   Erzeugen einer User-DLL zum Ersatz der Tektronix AG2-Dummyroutinen.
C   Wie AG2PART6.FOR, ersetzt jedoch USESET durch Code aus AG2DEMO4.FOR
C
C   Nach Erzeugung durch die Watcom-IDE muß das File D4USER.DLL in
C   Graph2Du.DLL umbenannt werden.
C
C   Anmerkung: die noch in dem ursprünglichen File AG2DEMO4.FOR enthaltene
C   Definition von USESET wird jetzt nicht verwendet, stört jedoch auch
C   nicht, da die durch die GRAPH2D.DLL zu verwendenden Module durch das
C   Importfile festgelegt werden.

       subroutine useset(fnum,iwidth,nbase,labeli)
       dimension labeli(2)
       dimension iascil(27)
       save iascil
       data iascil/32,32,32,32,54,37,32,56,37,32,57,37,
     1  49,52,37,52,51,37,54,56,37,32,32,32,32,32,32/
       iw= iwidth-3
       do 100 i=1,iw
100    labeli(i)=32
       ic= (ifix(fnum)*3)+1
       iw=iw+1
       do 200 i=iw,iwidth
       labeli(i)=iascil(ic)
200    ic=ic+1
       return
       end
