--U.A. Midfielder
function c72491806.initial_effect(c)
	--special summon
	local e1=Effect.CreateEffect(c)
	e1:SetType(EFFECT_TYPE_FIELD)
	e1:SetCode(EFFECT_SPSUMMON_PROC)
	e1:SetProperty(EFFECT_FLAG_UNCOPYABLE)
	e1:SetRange(LOCATION_HAND)
	e1:SetCountLimit(1,72491806)
	e1:SetCondition(c72491806.spcon)
	e1:SetOperation(c72491806.spop)
	c:RegisterEffect(e1)
	local e2=Effect.CreateEffect(c)
	e2:SetDescription(aux.Stringid(22192994,0))
	e2:SetType(EFFECT_TYPE_QUICK_O)
	e2:SetCode(EVENT_FREE_CHAIN)
	e2:SetRange(LOCATION_MZONE)
	e2:SetCountLimit(1)
	e2:SetTarget(c72491806.target)
	e2:SetOperation(c72491806.operation)
	c:RegisterEffect(e2)
end
function c72491806.spfilter(c)
	return c:IsFaceup() and c:IsSetCard(0xb2) and not c:IsCode(72491806) and c:IsAbleToHandAsCost()
end
function c72491806.spcon(e,c)
	if c==nil then return true end
	return Duel.GetLocationCount(c:GetControler(),LOCATION_MZONE)>-1
		and Duel.IsExistingMatchingCard(c72491806.spfilter,c:GetControler(),LOCATION_MZONE,0,1,nil)
end
function c72491806.spop(e,tp,eg,ep,ev,re,r,rp,c)
	Duel.Hint(HINT_SELECTMSG,tp,HINTMSG_RTOHAND)
	local g=Duel.SelectMatchingCard(tp,c72491806.spfilter,tp,LOCATION_MZONE,0,1,1,nil)
	Duel.SendtoHand(g,nil,REASON_COST)
end
function c72491806.thfilter(c,e,tp)
	return c:IsFaceup() and c:IsSetCard(0xb2) and not c:IsCode(72491806) and c:IsAbleToHand()
	and Duel.IsExistingMatchingCard(c72491806.tffilter,tp,LOCATION_HAND,0,1,nil,e,tp,c:GetCode())
end
function c72491806.tffilter(c,e,tp,code)
	return c:IsSetCard(0xb2) and not c:IsCode(code) and c:IsCanBeSpecialSummoned(e,0,tp,false,false) 
end
function c72491806.target(e,tp,eg,ep,ev,re,r,rp,chk,chkc)
if chkc then return chkc:IsLocation(LOCATION_MZONE) and chkc:IsControler(tp) and c72491806.thfilter(chkc) end
	if chk==0 then return Duel.IsExistingTarget(c72491806.thfilter,tp,LOCATION_MZONE,0,1,nil,e,tp) end 
	Duel.Hint(HINT_SELECTMSG,tp,HINTMSG_TODECK)
	local g=Duel.SelectTarget(tp,c72491806.thfilter,tp,LOCATION_MZONE,0,1,1,nil,e,tp)
	Duel.SetOperationInfo(0,CATEGORY_TOHAND,g,1,0,0)
	Duel.SetOperationInfo(0,CATEGORY_SPECIAL_SUMMON,nil,1,tp,LOCATION_HAND)
end
function c72491806.operation(e,tp,eg,ep,ev,re,r,rp)
	local sc=Duel.GetFirstTarget()
	Duel.SendtoHand(sc,nil,REASON_EFFECT)
	if Duel.GetLocationCount(tp,LOCATION_MZONE)<=0 then return end
	Duel.Hint(HINT_SELECTMSG,tp,HINTMSG_SPSUMMON)
	local h=Duel.SelectMatchingCard(tp,c72491806.tffilter,tp,LOCATION_HAND,0,1,1,nil,e,tp,sc:GetCode())
	if h:GetCount()>0 then
		Duel.SpecialSummon(h,0,tp,tp,false,false,POS_FACEUP)
	end
end
