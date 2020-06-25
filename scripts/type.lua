--FUNÇÕES DE CONFIGURAÇÃO DO USUÁRIO

--Referências:
--http://www.malinc.se/math/latex/basiccodeen.php#:~:text=The%20code%20%5Ctimes%20is%20used,never%20be%20negative%20by%20definition.
--https://www.overleaf.com/learn/latex/Integrals,_sums_and_limits
--https://tex.stackexchange.com/questions/103988/rightarrow-with-text-above-it
--https://www.overleaf.com/learn/latex/lists
--Inclusão de arquivos .tex em outros : https://www.overleaf.com/learn/latex/Management_in_a_large_project#:~:text=The%20standard%20tools%20to%20insert,are%20%5Cinput%20and%20%5Cinclude%20.&text=Use%20this%20command%20in%20the,%5Cend%7Bdocument%7D%20).

table = require("table")
string = require("string")
utf8 = require("utf8")

--Váriaveis Globais

Precision = 3 -- decimal places precision


--OPERAÇÕES MATEMÁTICAS BÁSICAS

function add(a,b)
    return tostring(a).."+"..tostring(b)
end

function sub(a,b)
    return tostring(a).."-"..tostring(b)
end

function neg(a)
    return "(-"..a..")"
end

function mul(a,b)
    return tostring(a).."\\cdot "..tostring(b)
end

function sca(a,b)
    return tostring(a)..tostring(b)
end

function div(a,b)
    return "\\frac{"..tostring(a).."}{"..tostring(b).."}"
end

function pow(a,b)
    return tostring(a).."^{"..tostring(b).."}"
end

function nroot(a,b)
    if a == " " then
        return "\\sqrt{"..tostring(b).."}"
    else    
        return "\\sqrt["..tostring(a).."]{"..tostring(b).."}"
    end
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

function degree(a)
    return tostring(a).."^{\\circ}"
end

--OPERAÇÕES DE LÓGICA

function greaterThan(a,b)
    return tostring(a)..">"..tostring(b)
end

function greaterOrEqual(a,b)
    return tostring(a).."\\geq"..tostring(b)
end

function lessThan(a,b)
    return tostring(a).."<"..tostring(b)
end

function lessOrEqual(a,b)
    return tostring(a).."\\leq"..tostring(b)
end

function notEqual(a,b)
    return tostring(a).."\\neq"..tostring(b)
end

function f_or(a,b)
    return tostring(a).."\\lor"..tostring(b)
end

function f_and(a,b)
    return tostring(a).."\\land"..tostring(b)
end

function f_xor(a,b)
    return tostring(a).."\\oplus"..tostring(b) 
end

function f_not(a)
    return tostring(a).."\\lnot"
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

function lim(direction,in_relation_to,func,to) -- receives 3 or 4 arguments depending if a direction was specified
    if to == nil then -- when receveing 3 args, "to" is nil, hence, we shift the arguments accordingly
        to = func
        func = in_relation_to
        in_relation_to = direction
        direction = nil
    end

    if to == "∞" then
        to = "\\infty"
    end

    if direction == nil then
        return "\\lim_{"..tostring(in_relation_to).."\\to "..tostring(to).."} "..tostring(func)

    else -- if direction was specified
        if direction == "right" then -- correct character
            direction = "+"
        elseif direction == "left" then
            direction = "-"
        end
        
        return "\\lim_{"..tostring(in_relation_to).."\\to "..tostring(to).."^{"..tostring(direction).."}".."} "..tostring(func)
    end        
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
        return expression.."="..end_expression
    else
        return expression.."="..end_expression
    end        
    -- expression é o lado direito da avaliação, symb_expression são os argumentos sob a seta, end_expression é o resultado da avaliação
end

function def(a,b)
    return tostring(a).."="..tostring(b)
end

function eval(a,b)
    num = tonumber(b)

    if num~=nil then -- if "b" is number
        formatter = "%."..Precision.."e" -- to be formatted as scientific notation
        b = string.format(formatter,num)
        exp = string.sub(b,string.find(b,"e[0-9%+%-]+")) -- exponent
        exp = tonumber(string.sub(exp,string.find(exp,"[%+%-%d]+"))) -- just the number
        if exp ~= 0 then
            b = string.gsub(b,"e[%+%-%d]+","\\cdot 10^{") -- swap the "e" by the base 10 latex format
            b = b..tostring(exp).."}" -- end the string
        else -- when the exponent is 0, therefore, no need to the base 10 notation
            b = string.gsub(b,"e[%+%-%w]+","") -- remove the exponent
        end

    end
    
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

-- Tabela e função para conversão de simbolos unicode para notação latex

local symbols = {
    ["∞"] = "\\infty",
    ["π"] = "\\pi",
    ["Π"] = "\\Pi",
    ["Ω"] = "\\Omega",
    ["ω"] = "\\omega",
    ["φ"] = "\\phi",
    ["ϕ"] = "\\varphi",
    ["Φ"] = "\\Phi",
    ["ξ"] = "\\xi",
    ["σ"] = "\\sigma",
    ["Σ"] = "\\Sigma",
    ["α"] = "\\alpha",
    ["ς"] = "\\varsigma",
    ["τ"] = "\\tau",
    ["ζ"] = "\\zeta",
    ["β"] = "\\beta",
    ["γ"] = "\\gamma",
    ["Γ"] = "\\Gamma",
    ["δ"] = "\\delta",
    ["Δ"] = "\\Delta",
    ["ε"] = "\\epsilon",
    ["η"] = "\\eta",
    ["θ"] = "\\theta",
    ["Θ"] = "\\Theta",
    ["κ"] = "\\kappa",
    ["λ"] = "\\lambda",
    ["Λ"] = "\\Lambda",
    ["ρ"] = "\\rho",
    ["ν"] = "\\nu",
    ["Ψ"] = "\\psi",
    ["ψ"] = "\\Psi",
    ["Χ"] = "\\chi",
    ["∓"] = "\\mp",
	["±"] = "\\pm"
}

local function math_symbol_check(symb) -- check if there is characters in a string that contain characters present in the global table "symbols" and swap then by the latex string equivalent
    local begin, ending  = string.find(symb,utf8.charpattern) -- search utf8 char

    while (begin ~= nil) do
        
        local character = string.sub(symb,begin,ending)
        
        if ((ending-begin) >= 1) and (symbols[character] ~= nil) then -- if it is a non ascii utf8 char, byte len > 1, and if the symbol is not present in symbols table, then we dont want to substitute it by a latex string equivalent
            symb = string.gsub(symb,character,symbols[character])
        end

        local cursor = ending + 1
        begin,ending  = string.find(symb,utf8.charpattern,cursor) 
    end
    
    return symb
end

function leg(a,b,c)
    a = math_symbol_check(a);
    b = math_symbol_check(b);
    c = math_symbol_check(c);

    expression = tostring(a)

    if b ~= " " then
        expression = expression.."_{"..tostring(b).."}"
    end
    if c ~= " " then
        expression = expression.."^{"..tostring(b).."}"
    end

    return expression
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
        v, n = string.gsub(v,"⨌","") --if it have the special character ⨌ then it is a text string, otherwise it is a math string inside a text string, therefore receive "$..$" to indicate math inline 
        if n > 0 then
            expression = expression..tostring(v)
        else
            expression = expression.." $"..tostring(v).."$ "
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

--DEFINIÇÕES DE IMAGEM

local IMAGE_SCALE_STANDARD = "height=4cm" -- escala padrão de imagem

local img_ref = 0 -- conta quantas imagens foram usadas
                  -- se usa na geração de titulos e outros campos automáticos

function img(filename,title,source,label,scale)
    if filename == "(null)" then
        return ""
    end

    if title == nil then ------- Em caso the se especificar só uma imagem sem titulo nem nada
        expression = [[
\begin{figure}[h!]
    \centering
    \caption
    \includegraphics[_IMG_SCALE_]{_IMAGE_PATH_}
    
\label{_LABEL_}
\end{figure}
        ]]
        expression = string.gsub(expression,"_IMAGE_PATH_",filename)
        expression = string.gsub(expression,"_IMG_SCALE_",IMAGE_SCALE_STANDARD)
        expression = string.gsub(expression,"_LABEL_","fig:"..tostring(img_ref))

    else -------------------------- Em caso de se especificar imagem com todas as informações

        expression = [[
\begin{figure}[h!]
    \centering
    \caption{_TITULO_}
    \includegraphics[_IMG_SCALE_]{_IMAGE_PATH_}
            
Fonte: _FONTE_
\label{_LABEL_}
\end{figure}
        ]]
        expression = string.gsub(expression,"_TITULO_",title)        
        expression = string.gsub(expression,"_IMG_SCALE_",IMAGE_SCALE_STANDARD)
        expression = string.gsub(expression,"_IMAGE_PATH_",filename)
        expression = string.gsub(expression,"_FONTE_",source)
        expression = string.gsub(expression,"_LABEL_","fig:"..tostring(img_ref))
    end

    img_ref = img_ref + 1 -- incrementa contador de referência de imagem

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
    if expression == "" then -- there is no expression
        return
    end

    if type == "math" then
        format = "nline"
    elseif type == "text" then
        format = "text"
    elseif type == "image" then
        format = "text"
    else
        format = "inline"
    end

    fp:write(math_format(expression, format))
    fp:write("\n\n")
end