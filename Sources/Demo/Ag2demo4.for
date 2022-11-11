C Demo fuer Bar-Charts AG2 User Manual Seite A8
C   Linken mit L80 ag2demo4,bar,tek/s,forlib/s
C
       dimension bar1(7),bar2(7,3)
       dimension ilabel(22),jlabel(34),klabel(42),label(11),label1(5)
       data bar1/6.,1.,2.,3.,4.,5.,6./
       data bar2/6.,2.,18.,19.,25.,22.,33.,
     1 6.,50.,62.,60.,60.,76.,79.,
     2 6.,100.,100.,100.,100.,100.,100./
       data ilabel/72,73,71,72,69,83,84,32,80,69,82,67,69,78,84,
     1 32,79,70,32,85,83,69/
       data jlabel/69,86,65,76,85,65,84,73,79,78,32,79,70,32,
     1 73,78,68,85,83,84,82,73,65,76,32,82,69,83,79,85,82,67,69,83/
       data klabel/80,82,69,86,65,76,69,78,67,69,32,65,78,68,32,
     1  69,70,70,69,67,84,73,86,69,78,69,83,83,32,79,70,32,77,
     2 69,67,72,65,78,73,83,77,83/
       data label/73,78,32,66,85,83,73,78,69,83,83/
       data label1/76,69,86,69,76/
       call initt(120)
       call binitt
       call slimx(200,900)
       call slimy(300,720)
       call comset(ibasey(3),-1.)
       call ytics(8)
       call ywdth(20)
       call yfrm(1)
       call ymfrm(1)
       call xfrm(2)
       call frame
       call dlimx(0.,100.)
       call dlimy(0.,8.)
       call hbarst(it(1),30,20)
       call check(bar2,bar1)
       call dsplay(bar2,bar1)
       do 100 j=2,3
       call hbarst(it(j),30,20)
       call ledg(530,30,j)
100    call cplot(bar2(1,j),bar1)
       call ledg(530,30,3)
       call movabs(50,750)
       call vlabel(22,ilabel)
       call notate(400,30,5,label1)
       call notate(300,150,34,jlabel)
       call notate(250,700,42,klabel)
       call notate(440,650,11,label)
       call finitt
       end

       function it(i)
       integer itt(3)
       save itt
       data itt/12,2,0/
       it=itt(i)
       return
       end

       subroutine ledg(ix,iy,i)
       save j,ilx,ily
       data j/0/
       j=j+1
       if (j.gt.1) goto 100
       ilx=ix-80
       ily=iy+30
100    ilx=ilx+60
       call filbox(ilx,ily,ilx+60,ily+60,it(j),20)
       call movabs(ilx+20,ily-30)
       call anmode
       call toutpt(j+48)
       return
       end

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



