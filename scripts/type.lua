--FUNÇÕES DE CONFIGURAÇÃO DO USUÁRIO

--Referências:
--http://www.malinc.se/math/latex/basiccodeen.php#:~:text=The%20code%20%5Ctimes%20is%20used,never%20be%20negative%20by%20definition.
--https://www.overleaf.com/learn/latex/Integrals,_sums_and_limits

require("table")
require("string")

--OPERAÇÕES MATEMÁTICAS BÁSICAS

function add(a,b)
    return tostring(a).."+"..tostring(b)
end

function sub(a,b)
    return tostring(a).."-"..tostring(b)
end

function neg(a)
    return "(-("..a.."))"
end

function mul(a,b)
    return tostring(a).."\\cdot "..tostring(b)
end

function div(a,b)
    return "\\frac{"..tostring(a).."}{"..tostring(b).."}"
end

function pow(a,b)
    return tostring(a).."^{"..tostring(b).."}"
end

function sqrt(a)
    return "\\sqrt{"..tostring(a).."}"
end

function logx(a,b)
    return "log_{"..tostring(a).."} "..tostring(b)
end

function log(a)
    return "log "..tostring(a)
end

function ln(a)
    return "\\ln "..tostring(a)
end

--OPERAÇÕES DE CÁLCULO

function deri(in_relation_to,func,degree)
    if degree==" " then
        return "\\frac{d}{d"..tostring(in_relation_to).."} "..tostring(func)
    else
        return "\\frac{d^{"..tostring(degree).."}}{d"..tostring(in_relation_to).."^{"..tostring(degree).."}} "..tostring(func)
    end
end

function int(in_relation_to,func,lim_inf,lim_sup)
    if (lim_inf==" ") or (lim_sup==" ") then
        return "\\int "..tostring(func).." d"..tostring(in_relation_to)
    else
        return "\\int_{"..tostring(lim_inf).."}^{"..tostring(lim_sup).."} "..tostring(func).." d"..tostring(in_relation_to)
    end
end

function lim(in_relation_to,func,to)
    symbol = utf8.char(to)
    if symbol == "∞" then
        to = "\\infty"
    end
    return "\\lim_{"..tostring(in_relation_to).."\\to "..tostring(to).."} "..tostring(func)
end


--DEFINIÇÕES DE EQUAÇÃO

function symb(...)
    symb_expression = ""
    n = select("#",...)
    arg = {...}
    for i,v in ipairs(arg) do
        if i==1 then
            expression = tostring(v)
            --print(expression)
        elseif i==n then
            end_expression = tostring(v)
            --print(end_expression)
        elseif i==n-1 then
            symb_expression = symb_expression..tostring(v)
        else 
            symb_expression = symb_expression..tostring(v)..","
            --print(symb_expression)
        end
    end

    if symb_expression==" " then
        return expression.."\\rightarrow "..end_expression
    else
        return expression.."\\xrightarrow{"..symb_expression.."} "..end_expression
    end        
    -- expression é o lado direito da avaliação, symb_expression são os argumentos sob a seta, end_expression é o resultado da avaliação
end

function def(a,b)
    return tostring(a).."="..tostring(b)
end

function eva(a,b)
    return tostring(a).."="..tostring(b)
end

function equal(a,b)
    return tostring(a).."="..tostring(b)
end

function func(a,...)
    n = select("#",...)
    arg = {...}
    expression = ""
    for i,v in ipairs(arg) do
        if i==n then
            expression = expression..tostring(v)        
        else
            expression = expression..tostring(v)..","
        end
    end
    return tostring(a).."("..expression..")"
end

function rang(a,b,c)
    return tostring(a)..","..tostring(b).."..."..tostring(c)
end

--OUTROS OPERADORES E SIMBOLOS

function leg(a,b,c)
    if (b == " ") and (c ~= " ") then
        return tostring(a).."^{"..tostring(c).."}"
    elseif (b ~= " ") and (c == " ") then
        return tostring(a).."_{"..tostring(b).."}"
    elseif (b == " ") and (c == " ") then
        return tostring(a)
    else
        return tostring(a).."_{"..tostring(b).."}".."^{"..tostring(c).."}"
    end
end

function parens(a)
    return "("..tostring(a)..")"
end

--DEFINIÇÕES DE TEXTO

function Disc(...)
    n = select("#",...)
    arg={...}
    expression = "\\begin{itemize}\n"
    for i,v in ipairs(arg) do
        expression = expression.."\t\\item "..tostring(v).."\n"
    end
    expression = expression.."\\end{itemize}"
    return expression
end

function para(...)
    n = select("#",...)
    arg={...}
    expression = ""
    for i,v in ipairs(arg) do
        if i==n-1 then
            expression = expression..tostring(v)
        else        
            expression = expression..tostring(v)
        end
    end
    return expression
end

function text(...)
    n = select("#",...)
    arg={...}
    expression = ""
    for i,v in ipairs(arg) do
        if i==n then
            expression = expression..tostring(v)
        else
            expression = expression..tostring(v).."\n"
        end
    end
    return expression
end


--ÁREA DE CÓDIGO NATIVO, NÃO ALTERAR

-- format = "inline", "nline", "text"
function math_format(string, format)
    if format == "inline" then
        return "$"..string.."$"
    elseif format == "nline" then
        return "$$"..string.."$$"
    elseif format == "text" then
        return string
    end
end

function add_field(expression, type, fp)
    if type == "math" then
        format = "nline"
    elseif type == "text" then
        format = "text"
    else
        format = "inline"
    end

    fp:write(math_format(expression, format))
    fp:write("\n\n")
end

--print(  symb(deri(leg("t"," "," "),func(leg("H"," "," "),leg("t"," "," "))," "), ,deri(leg("t"," "," "),func(leg("H"," "," "),leg("t"," "," "))," "))  )

--fp = io.open("output.tex","w+")

--expression = add(5,sub(6,-2))

--print(math_format(expression,"inline"))

--add_field(expression,"math",fp)

--fp:close()