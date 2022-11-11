      program HDCOUT
C Gibt saemtliche Files des Typs BILD?.HDC auf einem GP100VC aus
C      Dr.-Ing. K. Friedewald
C
      logical PeekV,GetHDC
      logical ishort,inilen,inistr(5),finlen,finstr(2)
      logical FilNam(8)
      integer Count
      integer*4 ibit,iShift
      logical ibitl(4),OldByt
      integer ibit2(2)
      equivalence (ibit,ibitl(1),ibit2(1))
      data inilen,inistr/5,8,27,16,0,40/
      data finlen,finstr/2,13,15/
      data filnam/'B','I','L','D','A',' ',' ',' '/

      call tcsini
      call initt
c
c  Schleife ueber die Hardcopyfiles
c

5     if (gethdc (filnam)) goto 99

c
c  Initialisierung des Druckers
c
       write (2,200)
       do 10 ishort=1,inilen
10      call print (4,inistr(ishort))
c
c  Ausgabe
c
       idone= 0
       iSpalt= 0
20     continue
        iShift= 2.**(9.-idone)+.5
        iZeile= 199
        OldByt= 0
        Count= 0
25      if (iZeile.lt.0) goto 30
         ibit= 0
         ibitl(2)= PeekV (izeile*80+iSpalt)
         if (iSpalt.lt.79) ibitl(1)= PeekV(iZeile*80+iSpalt+1)
         iBit= iBit/iShift
C
C GP-100: Bit0 = oberste Nadel!
C
         if (ibitl(1).lt.0) ibit2(1)= ibit2(1)-128
         ibitl(3)= 0
         ibitl(4)= 64
         do 27 ishort=1,7
          iBitl(3)= ibitl(3)+ mod(iBitl(1),2)*ibitl(4)
          iBitl(1)= ibitl(1)/2
          ibitl(4)= ibitl(4)/2
27       continue
         ibit2(2)= ibit2(2)+128
C
C Druckwegoptimierung
C
         if ((ibitl(3).eq.OldByt).and.(Count.le.252)) goto 33
          if (count.eq.0) goto 35
           call print (4,26)
           call print (4,Count)
           call print (4,OldByt)
           Count= 0
35        continue
          OldByt= ibitl(3)
33       continue
         Count= Count+2
         iZeile= iZeile-1
         goto 25
30      continue
        idone= idone+7
        if (idone.lt.8) goto 40
         idone= idone-8
         iSpalt= iSpalt+1
40      continue
        if (count.eq.0) goto 50
         call print (4,26)
         call print (4,Count)
         call print (4,OldByt)
50      continue
        call print (4,13)
       if (iSpalt.lt.80) goto 20
C
C Drucker auf Seitenanfang
C
       do 60 i=1,finlen
60      call print (4,finstr(i))
       write (2,200)
C
C Naechstes File
C
       filnam(5)= Filnam(5)+1
      goto 5
99    call alpha
      write (1,110)

110   format (1x,'HDCout done.')
200   format (1h0,///)
      end

