#include "ClothesMatcher.h"
#include "ClothesMatchProperties.h"

ClothesMatcher::ClothesMatcher(ClothesMatchProperties * parent) :
    QObject(parent),
    props_(parent)
{
    setObjectName("ClothesMatcher");
    clear();
}

void ClothesMatcher::clear(void)
{
    origImage_ = QImage();
    eyes_ = Eyes();
    skinColor_ = AnyColor();
    upperRect_ = QRect();
    lowerRect_ = QRect();
    upperImage_ = QImage();
    lowerImage_ = QImage();
    upperCount_ = 0;
    lowerCount_ = 0;
    upperConfidence_ = 0;
    lowerConfidence_ = 0;
    upperHistogram_.clear();
    lowerHistogram_.clear();
    upperPrimary_ = CharacteristicColor();
    lowerPrimary_ = CharacteristicColor();
    upperSecondary_ = CharacteristicColor();
    lowerSecondary_ = CharacteristicColor();
} // clear()

bool ClothesMatcher::isMatch(void) const
{
    if ( ! props_)
        return false;
    if ( ! props_->upperCharacteristicColor().isNull()
            && upperRect_.isEmpty())
        return false;
    if ( ! props_->lowerCharacteristicColor().isNull()
            && lowerRect_.isEmpty())
        return false;

    unsigned upperThreshold = props_->getUpperConfidence();
    unsigned lowerThreshold = props_->getLowerConfidence();
    upperThreshold = upperThreshold ? upperThreshold : 500;
    lowerThreshold = lowerThreshold ? lowerThreshold : 500;
    return (0 == upperConfidence_ || upperConfidence_ >= upperThreshold)
                && (0 == lowerConfidence_ || lowerConfidence_ >= lowerThreshold);
}

void ClothesMatcher::setFace(const QImage & orig, const Eyes eyes)
{
    clear();
    origImage_ = orig;
    eyes_ = eyes;
}

QString ClothesMatcher::upperColorNames(void) const
{
    QString result;
    if (upperPrimary_.isValid())
        result = upperPrimary_.name();
    if (upperSecondary_.isValid())
        result += " & " + upperSecondary_.name();
    return result;
}

QString ClothesMatcher::lowerColorNames(void) const
{
    QString result;
    if (lowerPrimary_.isValid())
        result = lowerPrimary_.name();
    if (lowerSecondary_.isValid())
        result += " & " + lowerSecondary_.name();
    return result;
}

QString ClothesMatcher::lowerColorString(void) const
{
    QString result;
    if (lowerPrimary_.isValid())
        result = lowerPrimary_.toString();
    if (lowerSecondary_.isValid())
        result += " & " + lowerSecondary_.toString();
    return result;
}

QString ClothesMatcher::upperColorString(void) const
{
    QString result;
    if (upperPrimary_.isValid())
        result = upperPrimary_.toString();
    if (upperSecondary_.isValid())
        result += " & " + upperSecondary_.toString();
    return result;
}

void ClothesMatcher::calculate(void)
{
    upperConfidence_ = lowerConfidence_ = 0;
    if ( ! props_)
    {
        emit error("No ClothesMatcherProperties");
        return;
    }
    if (origImage_.isNull())
    {
        emit error("No Image for ClothesMatcher");
        return;
    }
    if (eyes_.isNull())
    {
        emit error("No Eye Locations for ClothesMatcher");
        return;
    }

    QColor upperTarget = props_->getUpperColor();
    upperRect_ = props_->upperRect(eyes_) & origImage_.rect();
    if ( ! upperRect_.isEmpty())
    {
        upperImage_ = CharacteristicColor::process(origImage_.copy(upperRect_),
                                                   skinColor_);
        upperHistogram_ = CharacteristicColor::histogram(upperImage_);
        upperPrimary_.setValue(upperHistogram_.sortBin(1));
        upperHistogram_.combine(upperPrimary_.value(), upperPrimary_.adjacent());
        qreal distance = upperPrimary_.hslDistance(upperTarget);
        if (upperHistogram_.countAt(upperPrimary_.value()) < upperHistogram_.total() / 2)
        {
            upperSecondary_.setValue(upperHistogram_.sortBin(2));
            distance = (upperSecondary_.hslDistance(QColor(upperTarget)) + 2 * distance) / 3.0;
        }
        if (upperTarget.isValid())
            upperConfidence_ = qBound(1, qRound(1000.0 / (3.0 * distance + 1.0)), 999);
    }

    QColor lowerTarget = props_->getLowerColor();
    lowerRect_ = props_->lowerRect(eyes_) & origImage_.rect();
    if ( ! lowerRect_.isEmpty())
    {
        lowerImage_ = CharacteristicColor::process(origImage_.copy(lowerRect_),
                                                   skinColor_);
        lowerHistogram_ = CharacteristicColor::histogram(lowerImage_);
        lowerPrimary_.setValue(lowerHistogram_.sortBin(1));
        lowerHistogram_.combine(lowerPrimary_.value(), lowerPrimary_.adjacent());
        qreal distance = lowerPrimary_.hslDistance(lowerTarget);
        if (lowerHistogram_.countAt(lowerPrimary_.value()) < lowerHistogram_.total() / 2)
        {
            lowerSecondary_.setValue(lowerHistogram_.sortBin(2));
            distance = (lowerSecondary_.hslDistance(lowerTarget) + 2 * distance) / 3.0;
        }
        if (lowerTarget.isValid())
            lowerConfidence_ = qBound(1, qRound(1000.0 / (3.0 * distance + 1.0)), 999);
    }
}
