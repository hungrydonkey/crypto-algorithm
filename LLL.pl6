use Math::Matrix;

my @base = (<19  2 32 46  3 33 >,
            <15 42 11  0  3 24 >,
            <43 15  0 24  4 16>,
            <20 44 44  0 18 15>,
            < 0 48 35 16 31 31>,
            <48 33 32  9  1 29> );
my @lbase;
my $α = 3/4;
my $sqr = ->$x {$x**2};

sub vector(@x,@y,$op?){
    with $op  {
        my @result;
        for 0..@x.end -> $i { @result.push(@x[$i] - @y[$i]); }
        return @result;
    }
    else{
        my $result = 0;
        for 0..@x.end -> $i {  $result += @x[$i] * @y[$i]; }
        return $result;
    }
}

sub reduce($k,$l,@lbase,@mu){
    if abs(@mu[$k;$l]) > 1/2 {
        @lbase[$k] = vector(@lbase[$k],map(->$x {$x * @mu[$k;$l].round},@(@lbase[$l])),'-'); 
        for 0..$l-1 -> $j { @mu[$k;$j] = @mu[$k;$j] - @mu[$k;$l].round * @mu[$l;$j]; }
        @mu[$k;$l] = @mu[$k;$l] - @mu[$k;$l].round;
    }
}

sub exchange($k,@lbase,@mu,@mo){
    my $z = @lbase[$k-1];
    @lbase[$k-1] = @lbase[$k];
    @lbase[$k] = $z;
    my $ν = @mu[$k;$k-1];
    my $δ = @mo[$k] + ($ν**2) * @mo[$k-1];
    @mu[$k;$k-1] = $ν * @mo[$k-1] / $δ;
    @mo[$k] = @mo[$k] * @mo[$k-1] / $δ;
    @mo[$k-1] = $δ;
    for 0..$k-2 -> $j {
        my $t = @mu[$k-1;$j];
        @mu[$k-1;$j] = @mu[$k;$j];
        @mu[$k;$j] = $t;
    }
    for $k+1..^@base.elems -> $i {
        my $ξ  = @mu[$i;$k];
        @mu[$i;$k] = @mu[$i;$k-1] - $ν * @mu[$i;$k];
        @mu[$i;$k-1] = @mu[$k;$k-1] * @mu[$i;$k] + $ξ;
    }
}

sub LLL(@base --> Array) {
    my @mu[@base.elems;@base.elems];
    my @mo;
    my @vbase;
    my @lbase;
    @lbase = @base.clone;
    loop (my $i = 0;$i < @base.elems; $i++){
        @vbase[$i] = @lbase[$i]; 
        for 0..$i-1 -> $j {
            @mu[$i;$j] = vector(@lbase[$i],@vbase[$j]) / @mo[$j];
            @vbase[$i] = vector(@vbase[$i],map(->$x {$x * @mu[$i;$j]},@(@vbase[$j])),'-');
        } 
        @mo[$i] = [+] (@(@vbase[$i])>>.$sqr);
    }
    my $k = 1;
    while $k < @base.elems {           
        #say 'K is ',$k+1;
        reduce($k,$k-1,@lbase,@mu);
        if @mo[$k] >= ($α - @mu[$k;$k-1]**2) * @mo[$k-1] {
            for (0..$k-2).reverse ->$l { reduce($k,$l,@lbase,@mu); }
            $k = $k + 1;
        }
        else {
            exchange($k,@lbase,@mu,@mo);
            #say 'swap';
            if $k > 1 { $k = $k - 1;}
        }
    }
    return @lbase;
}

sub Hadamard(@base) {   
    my $det = Math::Matrix.new([@base]).det;
    my $mog = 1;
    #say $det;
    for 0..^@base.elems -> $i {
        my $mo = 0;
        for 0..^@base.elems -> $j {
            $mo += @base[$i][$j] ** 2;
        }
        $mog *= $mo ** (1/2);
    }
    return (abs($det)/$mog)**(1/@base.elems);
}
@lbase =  LLL(@base);
say Math::Matrix.new([@lbase]);
say 'α is ',$α;
#say 'base\'s Hadamard ratio is ',Hadamard(@base);
#say 'lbase\'s Hadamard ratio is',Hadamard(@lbase);